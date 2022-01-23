#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "ICommandWriter.h"

class FileWriter : public ICommandWriter
{
    std::string _filename;
public:
    FileWriter() = default;
    void write(const std::string &) override;
    void setCreateBlockTime(const time_t &time) override;
};

#endif //FILEWRITER_H