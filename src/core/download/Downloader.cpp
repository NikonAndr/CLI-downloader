#include "core/download/Downloader.h"

void Downloader::download(const std::string& url, const std::string& output_path)
{
    FileWriter fw(output_path);
    HttpClient httpClient;

    auto callback = [&fw](const char* data, size_t size)
    {
        fw.write(data, size);
    };

    httpClient.download(url, callback);
}