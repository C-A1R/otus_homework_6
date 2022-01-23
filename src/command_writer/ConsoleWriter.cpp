
#include "ConsoleWriter.h"

#include <iostream>

void ConsoleWriter::write(const std::string &text) 
{
    std::cout << text << std::endl;
}

void ConsoleWriter::setCreateBlockTime(const time_t &)
{    
}

