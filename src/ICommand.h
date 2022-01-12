#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>
#include <memory>
#include <vector>


/**
 * @brief Интерфейс команд
 */
class ICommand
{
public:
    ICommand() = default;
    virtual ~ICommand() = default;
    virtual void exec() = 0;
    virtual std::string getStr() const = 0;
};

/**
 * @brief Обычная команда
 */
class Cmd : public ICommand
{
    const std::string _str;
public:
    Cmd(const std::string &str) : _str{str} {}
    void exec() override {}
    std::string getStr() const override
    {
        return _str;
    }
};

/**
 * @brief Макрокоманда, которая может состоять из нескольких обычных команд
 */
class MacroCmd : public ICommand
{   
    std::vector<std::unique_ptr<ICommand> > _commands;
public:
    MacroCmd() = default;
    void addCmd(std::unique_ptr<ICommand> cmd)
    {
        _commands.emplace_back(std::move(cmd));
    }
    void exec() override
    {
        for (const auto &c : _commands)
        {
            c->exec();
        }
    }
    std::string getStr() const override
    {
        std::string result;
        for (size_t i = 0; i < _commands.size(); ++i)
        {
            result += _commands.at(i)->getStr();
            if (i != _commands.size() - 1)
            {
                result += ", ";
            }
        }
        return result;
    }
};

#endif //ICOMMAND_H