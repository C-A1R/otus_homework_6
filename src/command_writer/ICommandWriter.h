#ifndef ICOMMANDWRITER_H
#define ICOMMANDWRITER_H

#include <string>

class ICommandWriter
{
public:
    ICommandWriter() = default;
    virtual ~ICommandWriter() = default;
    virtual void write(const std::string &text) = 0;
    virtual void setCreateBlockTime(const time_t &time) = 0;
};



#endif //ICOMMANDWRITER_H