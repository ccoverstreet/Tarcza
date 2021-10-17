#include <iostream>
#include <vector>
#include <fstream>

#define WITHOUT_NUMPY 1
#include "vendor/matplotlib-cpp/matplotlibcpp.h"
#include <Eigen/Dense>
namespace plt = matplotlibcpp;

#include <omp.h>

#include "./core/objparser.h"
#include "./core/tracer.h"


int main() {
	auto geometry = parseObjFile("Assembly_3.obj");
	printf("# of parts: %d\n", geometry.parts.size());
	printf("# of triangles: %d\n", geometry.triangles.size());

	// Create sources
	Ray unit_ray(7, 0, 3, -1, 0, -0.5);
	std::vector<Source> sources = {createSource(unit_ray, M_PI / 4, 100)};
	
	std::map<std::string, std::string> keywords;
	keywords.insert(std::pair<std::string, std::string>("color", "c"));

	//plt::set_aspect_equal();

	for (auto tri : geometry.triangles) {
		std::vector<float> x = {tri.v1.x(), tri.v2.x(), tri.v3.x(), tri.v1.x()};
		std::vector<float> y = {tri.v1.y(), tri.v2.y(), tri.v3.y(), tri.v1.y()};
		std::vector<float> z = {tri.v1.z(), tri.v2.z(), tri.v3.z(), tri.v1.z()};

		plt::plot3(x, y, z, keywords, 1);
	}

	keywords["color"] = "r";
	keywords["linewidth"] = "0.1";


	float k = 10;
	for (auto ray : sources[0].rays) {
		std::vector<float> x = {ray.pos.x(), ray.pos.x() + k * ray.dir.x()};
		std::vector<float> y = {ray.pos.y(), ray.pos.y() + k * ray.dir.y()};
		std::vector<float> z = {ray.pos.z(), ray.pos.z() + k * ray.dir.z()};

		plt::plot3(x, y, z, keywords, 1);
	}

	plt::xlim(-10, 10);
	plt::ylim(-10, 10);
	std::vector<float> x = {0, 0};
	std::vector<float> y = {0, 0};
	std::vector<float> z = {-8, 8};
	plt::plot3(x, y, z, keywords, 1);
	plt::xlabel("x");
	plt::ylabel("y");
	//plt::zlim(-10, 10);
	plt::show();

	plt::detail::_interpreter::kill();

	return 0;
}
