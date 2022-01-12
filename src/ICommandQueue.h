#ifndef ICOMMANDQUEUE_H
#define ICOMMANDQUEUE_H

#include "ICommand.h"
#include "IFileWriter.h"

#include <iostream>
#include <queue>
#include <stack>
#include <ctime>


/**
 * @brief Интерфейс очереди команд
 */
class ICommandQueue
{
public:
    ICommandQueue() = default;
    virtual ~ICommandQueue() = default;

    /**
     * @brief Добавить команду в очередь
     */
    virtual void push(std::unique_ptr<ICommand> cmd) = 0;

    /**
     * @brief Начать запись макро-команды 
     */
    virtual void startMacros() = 0;

    /**
     * @brief Закончить запись макро-команды 
     */
    virtual void finishMacros() = 0;

    /**
     * @brief Количество команд в очереди
     */
    virtual size_t size() const = 0;

    /**
     * @brief Есть ли начатая макрокоманда
     */
    virtual bool haveStartedMacroses() const = 0;

private:
    /**
     * @brief Выполнить всю очередь команд
     */
    virtual void execCommands() = 0;
};


/**
 * @brief Очередь команд
 */
class CmdQueue : public ICommandQueue
{
    const size_t _blockSize;
    std::queue<std::unique_ptr<ICommand> > _commands;
    std::stack<std::unique_ptr<MacroCmd> > _macroCommands;
    std::unique_ptr<IFileWriter> _writer;
    time_t createBlockTime {0};
    
public:
    CmdQueue(size_t blockSize, std::unique_ptr<IFileWriter> writer) : _blockSize{blockSize}
    {
        _writer.swap(writer);
    }

    void push(std::unique_ptr<ICommand> cmd) override
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
                createBlockTime = std::time(0);
            }
            _commands.emplace(std::move(cmd));
        }

        if (_commands.size() == _blockSize)
        {
            execCommands();
        }
    }

    void startMacros()
    {
        if (_blockSize == 0)
        {
            return;
        }
        if (!haveStartedMacroses())
        {
            execCommands();
            createBlockTime = std::time(0);
        }
        _macroCommands.emplace(std::make_unique<MacroCmd>());
    }

    void finishMacros()
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

    virtual size_t size() const override
    {
        return _commands.size();
    }

    bool haveStartedMacroses() const override
    {
        return !_macroCommands.empty();
    }

private:
    void execCommands() override
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
        std::cout << bulkStr << std::endl;
        if (!_writer)
        {
            return;
        }
        _writer->write("bulk" + std::to_string(createBlockTime) + ".log", bulkStr);
        createBlockTime = 0;
    }
};

#endif //ICOMMANDQUEUE_H