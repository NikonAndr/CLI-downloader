#pragma once

#include "core/file/FileWriter.h"
#include "core/http/HttpClient.h"

class Downloader
{
public:
    Downloader() = default;
    ~Downloader() = default;
    void download(const std::string& url, const std::string& output_path);
};