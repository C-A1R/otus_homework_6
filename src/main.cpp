#include "command/Cmd.h"
#include "command/MacroCmd.h"
#include "command_queue/CmdQueue.h"
#include "command_writer/ConsoleWriter.h"
#include "command_writer/FileWriter.h"

#include <iostream>
#include <sstream>
#include <cassert>


int main (int argc, char **argv)
{
    assert(argc == 2);
    size_t queueSize {0};
    std::stringstream sstr(argv[1]);
    sstr >> queueSize;
    assert(queueSize > 0);

    auto commands = std::make_unique<CmdQueue>(queueSize);
    commands->addCommandWriter(std::make_unique<ConsoleWriter>());
    commands->addCommandWriter(std::make_unique<FileWriter>());
    for (std::string line; std::getline(std::cin, line);)
    {
        if (line.empty())
        {
            commands->push(nullptr);
        }
        else if (line == "{")
        {
            commands->startMacros();
        }
        else if (line == "}")
        {
            commands->finishMacros();
        }
        else
        {
            commands->push(std::make_unique<Cmd>(line));
        }
    }

    return 0;
}
