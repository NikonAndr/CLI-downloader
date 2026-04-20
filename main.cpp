#include "core/download/Downloader.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: ./build/downloader <URL> <output_file> <threads_num>";
        return 1;
    }

    try 
    {
        size_t threads = std::stoul(argv[3]);
        if (threads == 0)
        {
            throw std::runtime_error("threads_num must be > 0");
        }

        Downloader downloader;
        downloader.download(argv[1], argv[2], threads);

        std::cout << "Download Complete!\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}