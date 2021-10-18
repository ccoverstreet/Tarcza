#include <iostream>
#include <vector>
#include <fstream>

/*
#define WITHOUT_NUMPY 1
#include "vendor/matplotlib-cpp/matplotlibcpp.h"
#include <Eigen/Dense>
namespace plt = matplotlibcpp;
*/

#include <omp.h>
#include <yaml.h>

#include "./core/objparser.h"
#include "./core/tracer.h"


int main() {
	auto geometry = parseObjFile("testfiles/Assembly_3.obj");
	printf("# of parts: %d\n", geometry.parts.size());
	printf("# of triangles: %d\n", geometry.triangles.size());

	// Create sources
	Ray unit_ray(7, 0, 3, -1, 0, -0.4);
	Ray unit_ray_2(-7, 0, 3, 1, 0, -0.5);

	//std::vector<Source> sources = {createSource(unit_ray, M_PI, 1E4),
	//	createSource(unit_ray_2, M_PI * 2, 100)};
	std::vector<Source> sources = {createSource(unit_ray, M_PI / 4, 1E8)};

	for (auto src : sources) {
		std::cout << src;
	}
	
	std::cout << "Saving setup gnuplot script\n";
	saveSetupGNUPlot("setup.gnuplot", geometry, sources);

	tarczaTracingRoutine(geometry, sources);

	return 0;
}
