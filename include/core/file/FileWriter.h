#pragma once
#include <stdexcept>
#include <fstream>
#include <mutex>

class FileWriter 
{
private:
    std::ofstream file;
    std::mutex mutex;
public:
    FileWriter(const std::string& filename);
    ~FileWriter() = default;
    void write(const char* data, size_t size);
    void write_at(const char* data, size_t size, size_t offset);
    void preallocate(size_t size);
};