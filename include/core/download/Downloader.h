#pragma once

#include "core/file/FileWriter.h"
#include "core/http/HttpClient.h"

struct Chunk
{
    size_t start;
    size_t end;
};

class Downloader
{
public:
    Downloader() = default;
    ~Downloader() = default;
    void download(const std::string& url, const std::string& output_path, size_t threads_num);
};