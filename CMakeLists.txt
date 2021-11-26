cmake_minimum_required(VERSION 3.13)

Project(Tarcza)

# Dependencies
add_subdirectory(vendor/yaml-cpp)
link_directories(AFTER vendor/yaml-cpp)
include_directories(vendor/yaml-cpp/include)
include_directories(vendor/eigen)

# Building Tarcza
add_executable(Tarcza)
add_subdirectory(src)
target_link_libraries(Tarcza yaml-cpp)

find_package(OpenMP) # OpenMP support
if (OpenMP_CXX_FOUND)
	target_link_libraries(Tarcza OpenMP::OpenMP_CXX)
endif()
