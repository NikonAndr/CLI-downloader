#pragma once 

#include "core/http/HttpClient.h"
#include "core/file/FileWriter.h"

class Worker
{
private:
    HttpClient httpClient;
    size_t downloaded = 0;

    const std::string& url;
    size_t start;
    size_t end;
    FileWriter& fw;

public:
    Worker(const std::string& url, size_t start, size_t end, FileWriter& fw);
    ~Worker() = default;
    void run();
};