FLAGS = -std=c++11 -fopenmp -O3
INCLUDES = -Ivendor/eigen -Ivendor/yaml-cpp/include
LIBRARYDIRS = -Lvendor/yaml-cpp/build
SOURCES = main.cpp core/*.cpp 
LIBYAML = vendor/yaml-cpp/build/libyaml-cpp.a

driver: *.cpp core/*.cpp core/*.h $(LIBYAML)
	g++ $(FLAGS) $(INCLUDES) $(SOURCES) $(LIBRARYDIRS) -lyaml-cpp -o driver

$(LIBYAML):
	mkdir -p vendor/yaml-cpp/build
	cd vendor/yaml-cpp/build && cmake .. && make

run: driver
	./driver input.yaml
