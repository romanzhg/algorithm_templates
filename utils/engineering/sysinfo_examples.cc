#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <cstdio>
#include <cstdlib>
#include <mach/mach.h>

#include <sys/resource.h>
#include <sys/time.h>

// Needs
// #include <cstdlib>
// #include <mach/mach.h>
// Related header
// https://opensource.apple.com/source/xnu/xnu-792/osfmk/mach/task_info.h.auto.html
void printCurrentMemUsageMacOS() {
  struct task_basic_info t_info;
  mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
  if (KERN_SUCCESS != task_info(mach_task_self(),
                                TASK_BASIC_INFO, (task_info_t)&t_info, 
                                &t_info_count)) {
    printf("failed to get current process memory usage\n");
  } else {
    printf("current process memory usage:\nvirtual size(KB): %lu, resident size(KB): %lu\n", 
           t_info.virtual_size / 1024, t_info.resident_size / 1024);
  }
}

void printCurrentMemUsageLinux() {
  struct rusage rusage_;
  int rtn = getrusage(RUSAGE_SELF, &rusage_);
  if (rtn == -1) {
    printf("failed to get current process memory usage\n");
  } else {
    printf("data size: %ld\n", rusage_.ru_idrss);
    printf("stack size: %ld\n", rusage_.ru_isrss);
    printf("data + stack size: %ld\n", rusage_.ru_isrss + rusage_.ru_idrss);
  }
}

int main(int argc, const char * argv[]) {
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                        (host_info64_t)&vm_stats, &count))
    {
        long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;

        long long used_memory = ((int64_t)vm_stats.active_count +
                                 (int64_t)vm_stats.inactive_count +
                                 (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
        printf("free memory: %lld\nused memory: %lld\n", free_memory, used_memory);
    }

    printCurrentMemUsageMacOS();
    printCurrentMemUsageLinux();
    return 0;
}
