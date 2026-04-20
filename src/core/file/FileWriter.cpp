#include "core/file/FileWriter.h"
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

void FileWriter::write_at(const char* data, size_t size, size_t offset)
{
    std::lock_guard<std::mutex> lock(mutex);

    file.seekp(offset);

    if (!file)
    {
        throw std::runtime_error("[FileWriter] ERROR: seek failed");
    }

    file.write(data, size);
    
    if (!file.good())
    {
        throw std::runtime_error("[FileWriter] ERROR: Can't write to file");
    }
}

void FileWriter::preallocate(size_t size)
{
    if (size == 0)
        return;

    file.seekp(size - 1);

    if (!file)
    {
        throw std::runtime_error("[FileWriter] ERROR: seek failed in preaaloc");
    }

    file.write("", 1);

    if (!file.good())
    {
        throw std::runtime_error("[FileWriter] ERROR: preallocate write failed");
    }
}