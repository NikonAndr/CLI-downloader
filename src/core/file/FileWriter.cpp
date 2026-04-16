#include "FileWriter.h"
#include <stdexcept>

FileWriter::FileWriter(const std::string& filename)
    : file(filename, std::ios::binary)
{
    if (!file.is_open())
    {
        throw std::runtime_error("[FileWriter] ERROR: Can't open file");
    }
    
}

void FileWriter::write(const char* data, size_t size)
{
    file.write(data, size);

    if (!file.good())
    {
        throw std::runtime_error("[FileWriter] ERROR: Can't write to file");
    }
}