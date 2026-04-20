#include "core/download/Worker.h"

#include <functional>
#include <iostream>

Worker::Worker(const std::string& url, size_t start, size_t end, FileWriter& fw) : 
    url(url), start(start), end(end), fw(fw) {}

void Worker::run()
{
    std::function<void(const char*, size_t)> onData =
        [&](const char* data, size_t size)
    {
        fw.write_at(data, size, start + downloaded);
        downloaded += size;

        //debug 
        std::cout << "[Worker " << id << "] DEBUG: onData cb write_at(" << start + downloaded << ")\n"; 
    };

    long range_start = static_cast<long>(start);
    long range_end = static_cast<long>(end);
    long req_status = 0;

    req_status = httpClient.request(HttpMethod::GET, url, &onData, nullptr, range_start, range_end);

    if (req_status >= 400)
    {
        throw std::runtime_error("[Worker] ERROR: http request failed");
    }
}

void Worker::set_id(size_t value)
{
    id = value;
}

size_t Worker::get_id()
{
    return id;
}

