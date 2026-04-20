#include "core/http/HttpClient.h"
#include <curl/curl.h>

#include <stdexcept>

struct CurlContext
{
    const std::function<void(const char*, size_t)>* onData;
    const std::function<void(const char*, size_t)>* onHeader;
};

static size_t header_callback(char* buffer, size_t size, size_t nitems, void* userdata)
{
    size_t real_size = size * nitems;

    auto* ctx = static_cast<CurlContext*>(userdata);

    if (ctx->onHeader)
    {
        (*ctx->onHeader)(buffer, real_size);
    }

    return real_size;
}

static size_t data_callback(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    size_t real_size = size * nmemb;

    const char* data = static_cast<const char*>(ptr);

    auto* ctx = static_cast<CurlContext*>(userdata);

    if (ctx->onData)
    {
        (*ctx->onData)(data, real_size);
    }

    return real_size;
}

long HttpClient::request(HttpMethod method, const std::string& url, const std::function<void(const char* data, size_t size)>* onData, const std::function<void(const char* data, size_t size)>* onHeader, long range_start, long range_end)
{
    if (range_start != -1 && range_end != -1 && range_start > range_end)
    {
        throw std::runtime_error("ERROR [HttpClient]: wrong range");
    }

    if (method == HttpMethod::HEAD && range_start != -1)
    {
    throw std::runtime_error("ERROR [HttpClient]: HEAD cannot have range");
    }

    CURL *curl = curl_easy_init();
    long status = 0;
    std::string range;

    if (!curl)
    {
        throw std::runtime_error("ERROR: [HttpClient] curl init failed");
    }

    if (method == HttpMethod::HEAD)
    {
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    }

    CurlContext ctx{ onData, onHeader };

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Accept-Encoding: identity");
    headers = curl_slist_append(headers, "Connection: keep-alive");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, data_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ctx);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ctx);

    if (range_start != -1) 
    {
        range = std::to_string(range_start) + "-";
        if (range_end != -1)
        {
            range += std::to_string(range_end);
        }
        
        curl_easy_setopt(curl, CURLOPT_RANGE, range.c_str());
    }

    CURLcode result = curl_easy_perform(curl);
    

    if(result != CURLE_OK)
    {
        curl_easy_cleanup(curl);
        throw std::runtime_error(std::string("ERROR: [HttpClient] download problems: ") + curl_easy_strerror(result));
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

    if (headers)
    {
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);

    return status;
}
