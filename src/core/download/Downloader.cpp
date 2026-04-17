#include "core/download/Downloader.h"

#include <iostream>
#include <sstream>

void Downloader::download(const std::string& url, const std::string& output_path)
{
    FileWriter fw(output_path);
    HttpClient httpClient;

    size_t total_size = 0;
    size_t downloaded_bytes = 0;

    auto data_callback = [&fw, &downloaded_bytes, &total_size](const char* data, size_t size)
    {
        downloaded_bytes += size;
        fw.write(data, size);
        //Debug
        std::cout << "Downloaded: " << downloaded_bytes << "/" << total_size << " bytes\n";
    };

    auto header_callback = [&total_size](const char* buffer, size_t size)
    {
        std::string buffer_str(buffer, size);

        //debug
        std::cout << "header_cb: " << buffer_str <<" \n";
        
        size_t colon_pos = buffer_str.find(":");
        if (colon_pos == std::string::npos)
                return;

        std::string key = buffer_str.substr(0, colon_pos);
        std::string value = buffer_str.substr(colon_pos + 1);
        
        for (auto& c : key)
        {
            c = tolower(static_cast<unsigned char>(c));
        }

        if (key == "content-length")
        {
            size_t start = value.find_first_not_of(" \t\n\r");
            size_t end = value.find_last_not_of(" \n\t\r");

            if (start == std::string::npos || end == std::string::npos)
                return;

            value = value.substr(start, (end - start + 1));

            std::stringstream stream(value);
            stream >> total_size;
        }
    };

    httpClient.download(url, data_callback, header_callback);
    
    //debug 
    std::cout << "downloaded: " << downloaded_bytes << " of total of " << total_size << " bytes\n";
}