// macOS (AMDRadeonRDNA4's port): no monitor (crashpadclientmacos.cpp refuses to start one). What the shared code names
// is here, answering that there is nothing to read.

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "crashpadmonitorsystem.hpp"

namespace Crash::Monitor
{
    GameProcess::GameProcess(std::uint32_t id)
        : mId(id)
    {
    }

    void GameProcess::Closing::close(const Handle) noexcept {}

    Ending GameProcess::end() const
    {
        return Ending::Failed;
    }

    bool GameProcess::isHeld() const
    {
        return false;
    }

    std::optional<std::uint32_t> GameProcess::exitCode() const
    {
        return std::nullopt;
    }

    bool GameProcess::isGone() const
    {
        return true;
    }

    bool GameProcess::readWhole(const SnapshotStamp&, const std::function<bool(const crashpad::ProcessSnapshot&)>&) const
    {
        return false;
    }

    std::int32_t runHandler(std::vector<std::string>&, const HandlerParts&)
    {
        return 1;
    }

    std::string describeException(const crashpad::ExceptionSnapshot&, std::uint32_t)
    {
        return {};
    }

    std::string describeExitCode(std::uint32_t code)
    {
        return std::to_string(code);
    }

    bool isCrashCode(std::uint32_t)
    {
        return false;
    }
}
