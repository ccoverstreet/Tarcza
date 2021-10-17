driver: *.cpp core/*.cpp core/*.h
	g++ -std=c++11 -fopenmp -O3 -I/usr/include/python3.9 -Ivendor/eigen -lpython3.9 -I/usr/lib/python3.9/site-packages/numpy/core/include/ *.cpp core/*.cpp -o driver

run: driver
	./driver
