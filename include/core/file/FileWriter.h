#pragma once
#include <stdexcept>
#include <fstream>

class FileWriter 
{
private:
    std::ofstream file;
public:
    FileWriter(const std::string& filename);
    ~FileWriter() = default;
    void write(const char* data, size_t size);
};