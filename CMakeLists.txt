cmake_minimum_required(VERSION 3.13)

Project(Tarcza)

# Dependencies
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Turn off yaml-cpp tests" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Turn off yaml-cpp tools" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Turn off yaml-cpp contrib" FORCE)
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
