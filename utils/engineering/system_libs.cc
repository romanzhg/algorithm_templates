// 1. Set memory limit.
#include <sys/resource.h>

void SetMemLimit(long mbs) {
  struct rlimit mem_limit;
  long bytes;

  bytes = mbs * (1024 * 1024);
  // rlim_cur is the soft limit, rlim_max is the hard limit, according to
  // the man page only setting the soft limit would be enough, by experiment
  // actually we need to set both.
  mem_limit.rlim_cur = bytes;
  mem_limit.rlim_max = bytes;
  setrlimit(RLIMIT_AS, &mem_limit);
}

// 2. Set cpu affinity. Set the current thread to run on a cpu.
void SetCpuAffinity(int cpu_index) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu_index, &cpuset);

  if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
    printf("failed to set cpu affinity.\n");
    exit(0);
  }
}

// 3. Time related functions.
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
extern TimePoint (*GetCurrentTime)();
TimePoint (*GetCurrentTime)() = &std::chrono::system_clock::now;

inline uint64_t GetTimeStampMs(const TimePoint &t) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count();
}

inline bool ElapsedTimeLongerThanMs(TimePoint prev, uint64_t duration_ms) {
  return GetCurrentTime() - prev > std::chrono::milliseconds(duration_ms);
}

inline bool ElapsedTimeShorterThanMs(TimePoint prev, uint64_t duration_ms) {
  return GetCurrentTime() - prev < std::chrono::milliseconds(duration_ms);
}
