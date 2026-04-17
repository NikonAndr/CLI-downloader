## 💾 CLI Downloader (C++)
A simple multithread-ready CLI tool for downloading files over HTTP/HTTPS

## About 
This project is a custom-built command-line downloader written in C++. The goal was not to compete with tools like aria2, but to understand:

* how HTTP transfers work under the hood 
* how data is streamed and written to disk 
* how to design a modular system in C++
* how to build a clean, extensible architecture

## ⚙️ Features (v0.1)

* Download files over HTTP/HTTPS
* Streaming download (no full file buffering in memory)
* Binary-safe file writing
* Header parsing (Content-Length)
* Progress tracking (bytes download)
* CLI interface

## 📐 Architecture

CLI (main.cpp)
    ↓
Downloader
    ↓
HttpClient (libcurl)
    ↓
FileWriter 

Each module has a single responsibility 

* HttpClient → handles HTTP communication
* FileWriter → writes binary data to disk 
* Downloader → orchestrates the process
* CLI (main) → UI

## Usage 
### Build
```
make
```
### Run 
```
./build/downloader <URL> <output_file>
```

## 🛠 Tech Stack

* C++17
* libcurl
* Makefile

## 📈 Future Plans

* Multithread downloads (chunk-based)
* Resume support 
* Retry mechanism
* Progress bar (CLI UI)
* Batch downloads
* Scheduler
