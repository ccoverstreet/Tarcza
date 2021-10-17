FLAGS = -std=c++11 -fopenmp -O3
INCLUDES = -Ivendor/eigen
SOURCES = main.cpp core/*.cpp

driver: *.cpp core/*.cpp core/*.h
	g++ $(FLAGS) $(INCLUDES) $(SOURCES) -o driver

run: driver
	./driver
