#include "core/download/Downloader.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <thread>

void Downloader::download(const std::string& url, const std::string& output_path, size_t threads_num)
{
    HttpClient httpClient;

    size_t total_size = 0;
    size_t downloaded_bytes = 0;
    bool range_supported = false;
    bool range_works = false;

    long head_request_status = 0;
    long body_request_status = 0;
    long range_test_status = 0;

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

    //range test request
    if (range_supported)
    {
        range_test_status = httpClient.request(HttpMethod::GET, url, nullptr, nullptr, 0, 0);

        if (range_test_status == 206)
        {
            range_works = true;
        }
    }

    //Multi-thread download
    if (total_size > 0 && range_supported && range_works)
    {
        std::vector<Chunk> chunks;
        bool failed = false;
        
        //edge cases
        if (threads_num == 0)
        {
            std::cout << "[Downloader] threads number can't be 0, setting threads num to 1\n";
            threads_num = 1;
        }

        if (total_size < threads_num)
        {
            std::cout << "[Downloader] threads number exceeds total size -> threads num = total_size\n";
            threads_num = total_size;
        }

        //log 
        std::cout << "Multi-Thread download ( " << threads_num << " threads ) ...\n";

        size_t chunk_size = total_size / threads_num;

        for (size_t i = 0; i < threads_num; i++)
        {
            Chunk chunk {i * chunk_size, (i + 1) * chunk_size - 1};

            if (i == threads_num -1)
            {
                chunk.end = total_size - 1;
            }

            chunks.push_back(chunk);
        }

        FileWriter fw(output_path);
        fw.preallocate(total_size);
        std::vector<std::thread> threads;
        
        for (const auto& chunk : chunks)
        {
            threads.emplace_back([&, chunk]()
            {
                try 
                {
                    Worker worker(url, chunk.start, chunk.end, fw);
                    worker.run();
                }
                catch (const std::exception& e)
                {
                    std::cerr << "[Downloader] THREAD ERROR: " << e.what() << std::endl;
                    failed = true;
                }
            });
        }

        for (auto& t : threads)
        {
            t.join();
        }

        if (failed)
        {
            throw std::runtime_error("[Downloader] One or more workers failed");
        }
    }
    //Single-thread download
    else
    {
        FileWriter fw(output_path);

        //log
        std::cout << "Switched to Single-Thread, downloading...\n";

        std::function<void(const char*, size_t)> data_callback =
        [&](const char* data, size_t size)
        {
            downloaded_bytes += size;
            fw.write(data, size);
        };

        body_request_status = httpClient.request(HttpMethod::GET, url, &data_callback, nullptr, -1, -1);

        if (body_request_status >= 400)
        {
            throw std::runtime_error("ERROR [Downloader]: body req status" + std::to_string(body_request_status));
        }
    }
}