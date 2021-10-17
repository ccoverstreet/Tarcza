// Object parser for Tarcza
#pragma once

#include <vector>
#include <fstream>
#include <iostream>

#include <Eigen/Dense>
#include <cstdint>

#include "./types.h"
#include "./helpers.h"


Geometry parseObjFile(const char *filename);
