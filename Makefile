build:
	g++ sorting.cpp -o sorting -std=c++11 -O3

run:
	./sorting
	
buildrun: build run
