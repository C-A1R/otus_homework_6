#ifndef IFILEWRITER_H
#define IFILEWRITER_H

#include <string>
#include <fstream>


class IFileWriter
{
public:
    IFileWriter() = default;
    virtual ~IFileWriter() = default;
    virtual void write(const std::string &filename, const std::string &text) = 0;
};

class CommandFileWriter : public IFileWriter
{
public:
    CommandFileWriter() = default;
    void write(const std::string &filename, const std::string &text) override
    {
        if (filename.empty() || text.empty())
        {
            return;
        }
        std::ofstream outfile(filename);
        outfile << text << std::endl;
        outfile.close();
    }
};

#endif //IFILEWRITER_H