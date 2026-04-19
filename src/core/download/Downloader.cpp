#include "core/download/Downloader.h"

#include <iostream>
#include <sstream>

void Downloader::download(const std::string& url, const std::string& output_path)
{
    HttpClient httpClient;

    size_t total_size = 0;
    size_t downloaded_bytes = 0;
    bool range_supported = false;
    long head_request_status = 0;
    long body_request_status = 0;

   std::function<void(const char*, size_t)> header_callback = 
        [&](const char* buffer, size_t size)
    {
        std::string buffer_str(buffer, size);
        
        size_t colon_pos = buffer_str.find(":");
        if (colon_pos == std::string::npos)
                return;

        std::string key = buffer_str.substr(0, colon_pos);
        std::string value = buffer_str.substr(colon_pos + 1);
        
        for (auto& c : key)
        {
            c = tolower(static_cast<unsigned char>(c));
        }

        size_t start = value.find_first_not_of(" \t\n\r");
        size_t end = value.find_last_not_of(" \n\t\r");

        if (start != std::string::npos && end != std::string::npos)
        {
            value = value.substr(start, (end - start + 1));
        }

        if (key == "content-length")
        {
            std::stringstream stream(value);
            stream >> total_size;
        }
        else if (key == "accept-ranges")
        {
            if (value == "bytes")
            {
                range_supported = true;
            }
        }
    };

    head_request_status = httpClient.request(HttpMethod::HEAD, url, nullptr, &header_callback, -1, -1);
    
    if (head_request_status >= 400)
    {
        throw std::runtime_error("ERROR [Downloader]: head req status" + std::to_string(head_request_status));
    }
    //debug 
    std::cout << "Total_size: " << total_size << "\nrange_support : " << (range_supported ? "true" : "false") << std::endl;

    if (total_size > 0 && range_supported)
    {
        std::cout << "MULTI-THREAD download\n";
    }
    else
    {
        FileWriter fw(output_path);

        std::function<void(const char*, size_t)> data_callback =
        [&](const char* data, size_t size)
        {
            downloaded_bytes += size;
            fw.write(data, size);
            //Debug
            std::cout << "Downloaded: " << downloaded_bytes << "/" << total_size << " bytes\n";
        };

        body_request_status = httpClient.request(HttpMethod::GET, url, &data_callback, nullptr, -1, -1);

        if (body_request_status >= 400)
        {
            throw std::runtime_error("ERROR [Downloader]: body req status" + std::to_string(body_request_status));
        }
    }

}