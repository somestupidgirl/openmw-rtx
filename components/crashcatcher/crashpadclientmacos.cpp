// macOS (AMDRadeonRDNA4's port): no crash catcher. The monitor is written against Crashpad's Linux and Windows
// handlers; `unreadable` answers why, so `install` refuses before it starts anything and the game runs on without one,
// its crashes in the system's own reports. The rest is never reached past that refusal.

#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "crashpadclientsystem.hpp"

namespace Crash::Client
{
    void keepConnectionToThisProcess() {}

    void prepareInstallingThread() {}

    std::string_view catchPastTheProcess(crashpad::CrashpadClient&, const std::filesystem::path&)
    {
        return {};
    }

    void hookEveryEnd() {}

    void lostHooks(std::vector<std::string_view>&) {}

    std::string describeReading()
    {
        return "no crash catcher on macOS";
    }

    std::string_view unreadable()
    {
        return "this macOS port has no crash catcher: its monitor is written for Linux and Windows";
    }

    bool monitorRuns()
    {
        return false;
    }

    void watchMonitor(std::uint32_t) {}

    std::string_view letMonitorRead(std::uint32_t)
    {
        return {};
    }

    void endAsCrash(std::string_view)
    {
        std::abort();
    }
}
