#include "./types.h"

#include <cmath>
#include <iostream>

Source createSource(Ray unit_ray, float cone_angle, size_t n_rays) {
	Source src = {unit_ray, cone_angle, n_rays, {}};
	src.rays = createSourceRays(unit_ray, cone_angle, n_rays);

	return src;
}

std::vector<Ray> createSourceRays(Ray unit_ray, float cone_angle, size_t n_rays) {
	// Create arrays for all samples
	std::vector<Ray> rays;
	auto dir = unit_ray.dir;
	std::cout << dir << "\n";
	auto pos = unit_ray.pos;
	float phi = M_PI * (3.0 - sqrt(5.0));

	float x_max = 1.0;
	float x_min = cos(cone_angle / 2);

	auto norm1 = dir.cross(Eigen::Vector3f(1, 0, 0));
	auto j = dir.cross(norm1);
	auto k = dir.cross(j);

	Eigen::Matrix<float, 3, 3> R {
		{dir.x(), j.x(), k.x()},
		{dir.y(), j.y(), k.y()},
		{dir.z(), j.z(), k.z()}
	};

	std::cout << R << "\n";

	std::cout << R * Eigen::Vector3f(1, 0, 0) << "\n";
	
	for (size_t i = 0; i < n_rays; i++) {
		float x = x_max - (i/float(n_rays - 1)) * (x_max - x_min);
		float radius = sqrt(1 - x * x);

		float theta = phi * i;

		float y = cos(theta) * radius;
		float z = sin(theta) * radius;

		Eigen::Vector3f v(x, y, z);
		Eigen::Vector3f res = R * v;

		rays.push_back(Ray(pos, res));
	}		

	return rays;
}
