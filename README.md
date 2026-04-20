## 💾 CLI Downloader (C++)
A custom-built command-line downloader written in C++ with support for multithreaded downloads using HTTP Range requests.

## About 
This project is a custom CLI downloader designed to explore how file transfers work under the hood.

The goal was not to compete with tools like aria2, but to deeply understand:

* how HTTP transfers work under the hood 
* how data is streamed and written to disk 
* how to design a modular system in C++
* how to build a clean, extensible architecture

## ⚙️ Features (v0.2)

* Download files over HTTP/HTTPS
* Streaming download (no full file buffering in memory)
* Binary-safe file writing
* Header parsing (Content-Length)
* Multithreaded downloads (HTTP Range-based)
* Chunk-based downloading with concurrent workers
* Automatic fallback to single-thread when range is unsupported
* Preallocated file writing (safe concurrent writes)
* CLI interface

## 📐 Architecture

CLI (main.cpp) <br />
    ↓ <br />
Downloader  <br />
    ↓ <br />
Worker (multi-thread)  <br />
    ↓ <br />
HttpClient (libcurl) <br />
    ↓ <br />
FileWriter 

Each module has a single responsibility 

* HttpClient → handles HTTP communication
* FileWriter → writes binary data to disk 
* Worker → downloads a specific byte range (chunk)
* Downloader → orchestrates the process
* CLI (main) → UI

## Usage 
### Build
```
make
```
### Run 
```
./build/downloader <URL> <output_file> <threads_num>
```
### Example
```
./build/downloader "http://speedtest.tele2.net/100MB.zip" test.zip 10
```

## 🧪 Tested on
* Large files (100MB+)
* Chrome installer (Google CDN)
* Wikimedia dumps (~300MB)
* Servers with and without HTTP Range support

## ⚠️ Limitations
* Multithreaded mode requires server support for HTTP Range (206)
* No retry mechanism yet (planned)
* No resume support yet (planned)

## 🛠 Tech Stack

* C++17
* libcurl
* Makefile

## 📈 Future Plans

* Retry mechanism
* Resume support 
* Progress bar (CLI UI)
* Batch downloads
* Scheduler
