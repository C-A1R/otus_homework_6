#include "FileWriter.h"

#include <fstream>

void FileWriter::write(const std::string &text)
{
    if (_filename.empty() || text.empty())
    {
        return;
    }
    std::ofstream outfile(_filename);
    outfile << text << std::endl;
    outfile.close();
    _filename.clear();
}

void FileWriter::setCreateBlockTime(const time_t &time) 
{
    _filename = "bulk" + std::to_string(time) + ".log";
}
