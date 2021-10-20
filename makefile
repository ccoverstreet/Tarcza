FLAGS = -std=c++11 -fopenmp -O3
INCLUDES = -Ivendor/eigen -Ivendor/yaml-cpp/include
LIBRARYDIRS = -Lvendor/yaml-cpp/build
SOURCES = main.cpp core/*.cpp 
LIBYAML = vendor/yaml-cpp/build/libyaml-cpp.a

Tarcza: *.cpp core/*.cpp core/*.h $(LIBYAML) attenuation/coefficients.h
	g++ $(FLAGS) $(INCLUDES) $(SOURCES) $(LIBRARYDIRS) -lyaml-cpp -o Tarcza

attenuation/coefficients.h: attenuation/data/* attenuation/coefficients_template.h
	cd attenuation && python3 pack.py

$(LIBYAML):
	mkdir -p vendor/yaml-cpp/build
	cd vendor/yaml-cpp/build && cmake .. && make

run: Tarcza
	./Tarcza input.yaml
