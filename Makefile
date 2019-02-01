build:
	g++ sorting.cpp -o sorting -std=c++11 -O3
buildrun:
	$(build)
	./sorting
