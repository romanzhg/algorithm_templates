#ifndef OJ_UTILS_ENGINEERING_TWO_WAP_MAP_H_
#define OJ_UTILS_ENGINEERING_TWO_WAP_MAP_H_

#include <map>

struct WindowedDropEvents {
  const static int kWindowLen = 128;
  int actually_received[kWindowLen];
  int expect_to_receive[kWindowLen];
  int event_pointer;
  WindowedDropEvents() {
    memset(actually_received, 0, sizeof(actually_received));
    memset(expect_to_receive, 0, sizeof (expect_to_receive));
    event_pointer = 0;
  }
  // Count of expected packets and actually received packets(raw packets, before REC recovery).
  void AddEvent(int received, int expected) {
    actually_received[event_pointer] = received;
    expect_to_receive[event_pointer] = expected;
    event_pointer = (event_pointer + 1) % kWindowLen;
  }
  double GetWindowedDropRate() {
    int a = 0, b = 0;
    for (int i = 0; i < kWindowLen; i++) {
      a += actually_received[i];
      b += expect_to_receive[i];
    }
    return (double) a / (double) b;
  }
};
class FecGroup {
 public:
  std::vector<NetPacket *> packets_;
  std::vector<TimePoint> arrived_at_;
  std::vector<bool> nacked_;
  std::vector<bool> returned_;
  int filled_packets_{};
  bool done_processing_{};
  uint64_t start_seq_num_{};
  FecType fec_type_;
  const FecTypeInfo& fec_info_;
  FecGroup(uint64_t start_seq_num, FecType fec_type)
      : filled_packets_(0), done_processing_(false), start_seq_num_(start_seq_num), fec_type_(fec_type),
        fec_info_(GetFecTypeInfo(fec_type)) {
    packets_.resize(fec_info_.total_packets, nullptr);
    arrived_at_.resize(fec_info_.total_packets, TimePoint::min());
    nacked_.resize(fec_info_.total_packets, false);
    returned_.resize(fec_info_.total_packets, false);
  };
  FecGroup() = delete;
  FecGroup &operator=(const FecGroup &o) = delete;
  FecGroup(const FecGroup &o) = delete;
  FecGroup(FecGroup &&o) = delete;
  ~FecGroup() {
    for (NetPacket *p : packets_) {
      if (p != nullptr) {
        free(p);
      }
    }
  }
  TimePoint GetFirstPacketArrivalTime() const {
    TimePoint rtn = GetCurrentTime();
    for (int i = 0; i < (int) packets_.size(); i++) {
      if (packets_[i] != nullptr) {
        if (arrived_at_[i] < rtn) {
          rtn = arrived_at_[i];
        }
      }
    }
    return rtn;
  }
  TimePoint GetLastPacketArrivalTime() const {
    TimePoint rtn = TimePoint::min();
    for (int i = 0; i < (int) packets_.size(); i++) {
      if (packets_[i] != nullptr) {
        if (arrived_at_[i] > rtn) {
          rtn = arrived_at_[i];
        }
      }
    }
    return rtn;
  }
  bool GotEnoughPackets() const {
    return filled_packets_ >= fec_info_.data_packet_count;
  }
  // TODO: fill in here.
  bool IsDataPacket() const {
    return false;
  }
  void ExtractData(std::list<std::pair<void *, uint32_t>> &to_pop) {
    if (done_processing_) {
      return;
    }
    for (int i = 0; i < fec_info_.data_packet_count; i++) {
      if (packets_[i] != nullptr && (returned_[i] == false)) {
        const NetPacketBody *body = (NetPacketBody *) (&packets_[i]->body);
        body->GetDataSegments(to_pop);
        returned_[i] = true;
      }
    }
    bool all_extracted = true;
    for (int i = 0; i < fec_info_.data_packet_count; i++) {
      all_extracted = all_extracted && returned_[i];
    }
    if (all_extracted) {
      done_processing_ = true;
    }
  }
  bool ShouldFecDecode() const {
    if (done_processing_) {
      return false;
    }
    bool all_data_arrived = true;
    for (int i = 0; i < fec_info_.data_packet_count; i++) {
      if (packets_[i] == nullptr) {
        all_data_arrived = false;
      }
    }
    if (all_data_arrived) {
      return false;
    }
    return GotEnoughPackets();
  }
  std::vector<uint64_t> GetPacketsToNack() {
    std::vector<uint64_t> rtn;
    for (int i = 0; i < (int) fec_info_.total_packets; i++) {
      if (packets_[i] == nullptr && !nacked_[i]) {
        rtn.push_back(start_seq_num_ + i);
        nacked_[i] = true;
      }
    }
    return rtn;
  }
};

struct CircularBuffer {
  CircularBuffer(int size) {
    for (int i = 0; i < size; i++) {
      free_blocks.push(i);
    }
  };
  void AddBlock(int i) {
    free_blocks.push(i);
  }
  // Returns false for success, true for failure.
  bool GetOneBlock(int &rtn) {
    if (free_blocks.empty()) {
      return true;
    }
    rtn = free_blocks.front();
    free_blocks.pop();
    return false;
  }
  // Append blocks to @param rtn.
  // Returns false for success, true for failure.
  bool GetBlocks(int size, std::vector<int> &rtn) {
    if (size == 0) {
      return false;
    }
    if (free_blocks.size() < size) {
      return true;
    }
    for (int i = 0; i < size; i++) {
      rtn.push_back(free_blocks.front());
      free_blocks.pop();
    }
    return false;
  }
  std::queue<int> free_blocks;
};
struct TwoWapMap {
  TwoWapMap() {};
  void AddMapping(uint64_t seq_num, int index) {
    seq_num_to_index[seq_num] = index;
    index_to_seq_num[index] = seq_num;
  }
  void RemoveMapping(int index) {
    assert(index_to_seq_num.find(index) != index_to_seq_num.end());
    uint64_t seq_num = index_to_seq_num[index];
    index_to_seq_num.erase(index);
    assert(seq_num_to_index.find(seq_num) != seq_num_to_index.end());
    seq_num_to_index.erase(seq_num);
  }
  int GetIndexBySeqNum(uint64_t seq_num) {
    if (seq_num_to_index.find(seq_num) == seq_num_to_index.end()) {
      return -1;
    } else {
      return seq_num_to_index[seq_num];
    }
  }
  std::map<uint64_t, int> seq_num_to_index;
  std::map<int, uint64_t> index_to_seq_num;
};

#endif //OJ_UTILS_ENGINEERING_TWO_WAP_MAP_H_
