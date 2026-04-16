#pragma once

#include <functional>
#include <string>

class HttpClient
{
public:
    HttpClient() = default;
    ~HttpClient() = default;
    void download(const std::string& url, const std::function<void(const char* data, size_t size)>& callback);
};