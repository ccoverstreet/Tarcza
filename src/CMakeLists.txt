cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)

target_sources(Tarcza PRIVATE
	main.cpp
	objparser.cpp
	tracer.cpp
	types.cpp)
