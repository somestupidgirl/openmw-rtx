#include "process.hpp"

#include <algorithm>
#include <array>
#include <cerrno>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <limits>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <vector>

#include <sched.h>
// NOLINTNEXTLINE(modernize-deprecated-headers): POSIX's `kill`, which <csignal> need not declare
#include <signal.h>
#include <mach-o/dyld.h>
#include <pthread.h>
#include <sys/sysctl.h>
#include <sys/wait.h>
#include <unistd.h>

#include "folder.hpp"

namespace Platform::Process
{
    bool setEnvironmentDefault(const char* name, const char* value)
    {
        if (std::getenv(name) != nullptr)
            return false;

        setenv(name, value, 0); // NOLINT(concurrency-mt-unsafe): before any thread starts, as the header says
        return true;
    }

    void setEnvironment(const char* name, const char* value)
    {
        setenv(name, value, 1); // NOLINT(concurrency-mt-unsafe): before any thread starts, as the header says
    }

    void setEnvironmentPath(const char* name, const std::filesystem::path& value)
    {
        setenv(name, value.c_str(), 1); // NOLINT(concurrency-mt-unsafe): before any thread starts, as the header says
    }

    std::optional<std::filesystem::path> environmentPath(const char* name)
    {
        const char* const value = std::getenv(name);
        if (value == nullptr)
            return std::nullopt;
        return std::filesystem::path(value);
    }

    std::optional<std::filesystem::path> executable()
    {
        // (macOS: dyld's path of the main image, its links resolved)
        std::uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);
        std::string path(size, '\0');
        if (_NSGetExecutablePath(path.data(), &size) != 0)
            return std::nullopt;
        path.resize(std::char_traits<char>::length(path.c_str()));
        std::error_code error;
        std::filesystem::path real = std::filesystem::canonical(path, error);
        return error ? std::filesystem::path(path) : real;
    }

    std::vector<std::string> commandLine(const std::int32_t argc, char** const argv)
    {
        return std::vector<std::string>(argv, argv + argc);
    }

    void attachParentConsole() {}

    bool standardErrorTakesColour()
    {
        return std::getenv("TERM") != nullptr && isatty(STDERR_FILENO) != 0;
    }

    std::string_view executableSuffix()
    {
        return "";
    }

    std::uint32_t currentId()
    {
        return static_cast<std::uint32_t>(getpid());
    }

    bool isRunning(const std::uint32_t id)
    {
        if (id == 0 || id > static_cast<std::uint32_t>(std::numeric_limits<pid_t>::max()))
            return false;

        // Signal nought checks and sends nothing; a process of another user answers "not permitted",
        // which is an answer about a process that exists.
        return kill(static_cast<pid_t>(id), 0) == 0 || errno == EPERM;
    }

    std::uint64_t currentThreadId()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg): Linux's syscall
        std::uint64_t id = 0;
        pthread_threadid_np(nullptr, &id);
        return id;
    }

    void restartOnHugePages(char** argv)
    {
        (void)argv;   // (glibc's tunable: Linux's alone)
    }

    bool refuseTransparentHugePages()
    {
        return false;
    }

    std::optional<float> hugePageShare()
    {
        return std::nullopt;
    }

    std::optional<std::uint64_t> compactionStalls()
    {
        return std::nullopt;
    }

    std::optional<AnonymousMemory> anonymousMemory()
    {
        return std::nullopt;
    }

    std::optional<PrivateMemoryHeld> privateMemoryHeld()
    {
        return std::nullopt;
    }

    std::optional<DriverMemory> driverMemory()
    {
        return std::nullopt;
    }

    std::size_t keepToPerformanceCores()
    {
        return 0;   // (no affinity on macOS: the scheduler's QoS decides)
    }

    std::size_t usableCpuCount()
    {
        return std::max<std::size_t>(1, std::thread::hardware_concurrency());
    }

    bool startedFromTerminal()
    {
        return isatty(STDIN_FILENO) != 0;
    }

    bool showsADesktop()
    {
        return true;
    }

    std::string shellWord(std::string_view text)
    {
        std::string word = "'";
        for (const char c : text)
            word += c == '\'' ? std::string("'\\''") : std::string(1, c);
        return word + "'";
    }

    CommandEnd runShell(const std::string& line)
    {
        // Runs the command line its caller built.
        // NOLINTNEXTLINE(bugprone-command-processor)
        const std::int32_t status = std::system(line.c_str());

        // No shell at all: what a shell answers for a command it could not run.
        if (status == -1)
            return CommandEnd{ .mExitCode = 127 };
        if (WIFSIGNALED(status))
            return CommandEnd{ .mSignal = WTERMSIG(status) };
        return CommandEnd{ .mExitCode = static_cast<std::uint32_t>(WEXITSTATUS(status)) };
    }
}
