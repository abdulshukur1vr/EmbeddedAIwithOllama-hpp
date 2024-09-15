CXX ?= g++
all: sample-cpp11 sample-cpp20
sample-cpp11:
	mkdir -p build
	$(CXX) src/main.cpp src/utils.cpp -Iinclude -o build/zaas -std=c++11
sample-cpp20:
	mkdir -p build_cpp20
	$(CXX) src/main.cpp src/utils.cpp -Iinclude -o build_cpp20/zaas -std=c++20
clean:
	rm -rf build
	rm -rf build_cpp20
