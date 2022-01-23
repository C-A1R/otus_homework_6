#ifndef CONSOLEWRITER_H
#define CONSOLEWRITER_H

#include "ICommandWriter.h"

class ConsoleWriter : public ICommandWriter
{
public:
    ConsoleWriter() = default;
    void write(const std::string &text) override;
    void setCreateBlockTime(const time_t &) override;
};


#endif //CONSOLEWRITER_H