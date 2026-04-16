#include "core/http/HttpClient.h"
#include <curl/curl.h>

#include <stdexcept>

static size_t write_callback(void* ptr, size_t size, size_t nmemb, void*userdata)
{
    size_t real_size = size * nmemb;

    const char* data = static_cast<const char*>(ptr);

    auto& cb = *static_cast<const std::function<void(const char*, size_t)>*>(userdata);

    cb(data, real_size);

    return real_size;
}

void HttpClient::download(const std::string& url, const std::function<void(const char* data, size_t size)>& callback)
{
    CURL *curl = curl_easy_init();

    if (!curl)
    {
        throw std::runtime_error("ERROR: [HttpClient] curl init failed");
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);

    CURLcode result = curl_easy_perform(curl);
    

    if(result != CURLE_OK)
    {
        curl_easy_cleanup(curl);
        throw std::runtime_error(std::string("ERROR: [HttpClient] download problems: ") + curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);
}