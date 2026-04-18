#pragma once

#include <functional>
#include <string>

enum class HttpMethod 
{
    GET,
    HEAD
};

class HttpClient
{
public:
    HttpClient() = default;
    ~HttpClient() = default;
    void download(const std::string& url, const std::function<void(const char* data, size_t size)>& onData, const std::function<void(const char* data, size_t size)>& onHeader);

    long request(HttpMethod method, const std::string& url, const std::function<void(const char* data, size_t size)>* onData, const std::function<void(const char* data, size_t size)>* onHeader, long range_start, long range_end);
};