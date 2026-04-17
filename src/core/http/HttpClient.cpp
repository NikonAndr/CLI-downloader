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

    ctx->onHeader->operator()(buffer, real_size);

    return real_size;
}
static size_t data_callback(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    size_t real_size = size * nmemb;

    const char* data = static_cast<const char*>(ptr);

    auto* ctx = static_cast<CurlContext*>(userdata);

    ctx->onData->operator()(data, real_size);

    return real_size;
}

void HttpClient::download(const std::string& url, const std::function<void(const char* data, size_t size)>& onData, const std::function<void(const char* data, size_t size)>& onHeader)
{
    CURL *curl = curl_easy_init();
    CurlContext ctx{ &onData, &onHeader };

    if (!curl)
    {
        throw std::runtime_error("ERROR: [HttpClient] curl init failed");
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, data_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ctx);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ctx);


    CURLcode result = curl_easy_perform(curl);
    

    if(result != CURLE_OK)
    {
        curl_easy_cleanup(curl);
        throw std::runtime_error(std::string("ERROR: [HttpClient] download problems: ") + curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);
}