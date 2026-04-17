#include "core/download/Downloader.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./build/downloader <URL> <output_file>";
        return 1;
    }

    try 
    {
        Downloader downloader;
        downloader.download(argv[1], argv[2]);

        std::cout << "Download Complete!\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}