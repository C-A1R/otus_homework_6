#include "CmdQueue.h"

#include <ctime>

#include "../command/Cmd.h"
#include "../command/MacroCmd.h"
#include "../command_writer/ICommandWriter.h"


CmdQueue::CmdQueue(size_t blockSize) : _blockSize(blockSize)
{
}

void CmdQueue::push(std::unique_ptr<ICommand> cmd)
{
    if (_blockSize == 0)
    {
        return;
    }
    if (nullptr == cmd)
    {
        execCommands();
        return;
    }

    if (!_macroCommands.empty())
    {
        _macroCommands.top()->addCmd(std::move(cmd));
    }
    else
    {
        if (_commands.empty())
        {
            _createBlockTime = std::time(0);
        }
        _commands.emplace(std::move(cmd));
    }

    if (_commands.size() == _blockSize)
    {
        execCommands();
    }
}

void CmdQueue::startMacros()
{
    if (_blockSize == 0)
    {
        return;
    }
    if (!haveStartedMacroses())
    {
        execCommands();
        _createBlockTime = std::time(0);
    }
    _macroCommands.emplace(std::make_unique<MacroCmd>());
}

void CmdQueue::finishMacros()
{
    if (_blockSize == 0 || _macroCommands.empty())
    {
        return;
    }

    auto currentMacros = std::move(_macroCommands.top());
    _macroCommands.pop();
    if (_macroCommands.size() == 0)
    {
        _commands.emplace(std::move(currentMacros));
        execCommands();
    }
    else
    {
        _macroCommands.top()->addCmd(std::move(currentMacros));
    }
}

size_t CmdQueue::size() const
{
    return _commands.size();
}

bool CmdQueue::haveStartedMacroses() const
{
    return !_macroCommands.empty();
}

void CmdQueue::addCommandWriter(std::unique_ptr<ICommandWriter> writer)
{
    _writers.emplace_back(std::move(writer));
}

void CmdQueue::execCommands()
{
    if (haveStartedMacroses())
    {
        _macroCommands.pop();
    }
    if (_commands.empty())
    {
        return;
    }

    std::string bulkStr {"bulk: "};
    while (!_commands.empty())
    {
        _commands.front()->exec();
        bulkStr += _commands.front()->getStr();
        _commands.pop();
        if (_commands.size() != 0)
        {
            bulkStr += ", ";
        }
    }
    for (const auto &w : _writers)
    {
        w->setCreateBlockTime(_createBlockTime);
        w->write(bulkStr);
    }
    _createBlockTime = 0;
}
