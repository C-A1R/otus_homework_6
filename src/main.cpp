#include "ICommandQueue.h"

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

    std::unique_ptr<ICommandQueue> commands = std::make_unique<CmdQueue>(queueSize, std::make_unique<CommandFileWriter>());
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
