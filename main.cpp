#include <iostream>
#include <vector>
#include <fstream>

#define WITHOUT_NUMPY 1
#include "vendor/matplotlib-cpp/matplotlibcpp.h"
#include <Eigen/Dense>
namespace plt = matplotlibcpp;

#include "./core/objparser.h"
#include "./core/tracer.h"


int main() {
	Eigen::MatrixXd m(2, 2);
	m(0, 0)	= 3;
	m(0, 0)	= 3;
	m(0, 0)	= 3;
	m(0, 0)	= 3;

	std::cout << m << "\n";

	Eigen::Vector3d a(1, 0, 0);
	Eigen::Vector3d b(0, 1, 0);

	std::cout << a.cross(b)<< "\n";

	auto geometry = parseObjFile("assembly_1.obj");
	printf("# of parts: %d\n", geometry.parts.size());
	printf("# of triangles: %d\n", geometry.triangles.size());

	std::map<std::string, std::string> keywords;
	keywords.insert(std::pair<std::string, std::string>("color", "c"));

	traceRayPath(geometry, Ray{Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(0, 1, 0)});

	for (auto tri : geometry.triangles) {
		std::vector<float> x  = {tri.v1[0], tri.v2[0], tri.v3[0], tri.v1[0]};
		std::vector<float> y = {tri.v1[1], tri.v2[1], tri.v3[1], tri.v1[1]};
		std::vector<float> z = {tri.v1[2], tri.v2[2], tri.v3[2], tri.v1[2]};

		plt::plot3(x, y, z, keywords, 1);
	}

	plt::show();

	plt::detail::_interpreter::kill();
	return 0;
}
