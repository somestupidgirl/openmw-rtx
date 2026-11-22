#include "thread.hpp"

#include <chrono>
#include <ctime>
#include <string>
#include <string_view>
#include <system_error>

#include <pthread.h>
#include <pthread/qos.h>

namespace Platform
{
    void nameThisThread(const std::string_view name)
    {
        // Linux refuses a name of sixteen bytes or more, terminator included, rather than cutting it.
        const std::string kept(name.substr(0, sThreadNameLength));
        pthread_setname_np(kept.c_str());   // (macOS names the calling thread only)
    }

    std::string nameOfThisThread()
    {
        char name[64] = {};
        pthread_getname_np(pthread_self(), name, sizeof(name));
        return name;
    }

    std::chrono::nanoseconds cpuTimeOfThisThread()
    {
        timespec spent{};
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &spent);
        return std::chrono::seconds(spent.tv_sec) + std::chrono::nanoseconds(spent.tv_nsec);
    }

    std::error_code lowerThisThreadToIdle()
    {
        // (macOS: the background QoS class, its scheduler's lowest, in place of SCHED_IDLE)
        return std::error_code(pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0), std::generic_category());
    }
}
