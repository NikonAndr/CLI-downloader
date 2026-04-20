CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lcurl

SRC = main.cpp \
	src/core/file/FileWriter.cpp \
	src/core/http/HttpClient.cpp \
	src/core/download/Worker.cpp \
	src/core/download/Downloader.cpp

OUT = build/downloader

all:
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -rf build

