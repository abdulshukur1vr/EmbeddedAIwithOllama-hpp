CXX ?= g++
all: sample-cpp11
sample-cpp11: src/main.cpp
	mkdir -p build
	$(CXX) src/main.cpp -Iinclude -o build/asak -std=c++11
clean:
	rm -rf build
