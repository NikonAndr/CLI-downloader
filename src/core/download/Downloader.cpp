#include "core/download/Downloader.h"

#include <iostream>
#include <sstream>

void Downloader::download(const std::string& url, const std::string& output_path)
{
    FileWriter fw(output_path);
    HttpClient httpClient;

    size_t total_size = 0;
    size_t downloaded_bytes = 0;

    auto data_callback = [&fw, &downloaded_bytes](const char* data, size_t size)
    {
        downloaded_bytes += size;
        fw.write(data, size);
        //Debug
        std::cout << "Downloaded: " << downloaded_bytes << " bytes\n";
    };

    auto header_callback = [&total_size](const char* buffer, size_t size)
    {
        std::string buffer_str(buffer, size);

        if (buffer_str.find("Content-Length:") != std::string::npos)
        {
            size_t colon_pos = buffer_str.find(":");
            if (colon_pos == std::string::npos)
                return;

            std::string content_length = buffer_str.substr(colon_pos + 1);

            size_t start = content_length.find_first_not_of(" \t\n\r");
            size_t end = content_length.find_last_not_of(" \n\t\r");

            if (start == std::string::npos || end == std::string::npos)
                return;

            content_length = content_length.substr(start, (end - start + 1));

            std::stringstream stream(content_length);
            stream >> total_size;
        }
    };

    httpClient.download(url, data_callback, header_callback);
}