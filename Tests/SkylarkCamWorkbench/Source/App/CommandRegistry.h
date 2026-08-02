#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace skylark::cam
{
struct CommandDescriptor { std::string id; std::string title; std::string tab; std::string group; bool enabled{true}; };
class CommandRegistry
{
public:
    using Handler = std::function<void()>;
    void RegisterCommand(CommandDescriptor descriptor, Handler handler);
    bool Execute(const std::string& commandId) const;
    const std::vector<CommandDescriptor>& GetCommands() const noexcept;
    std::vector<CommandDescriptor> GetCommandsForTab(const std::string& tab) const;
private:
    std::vector<CommandDescriptor> m_commands;
    std::map<std::string, Handler> m_handlers;
};
CommandRegistry CreateDefaultCommandRegistry();
}
