#include "./types.h"

#include <cmath>
#include <iostream>
#include <fstream>

// Returns emission area in radians
float coneAngleToArea(float cone_angle) {
	return 2*float(M_PI)*(1 - cos(cone_angle/2));
}

Source createSource(Ray unit_ray, float cone_angle, size_t n_rays) {
	Source src = {unit_ray, cone_angle, n_rays, coneAngleToArea(cone_angle),{}};
	src.rays = createSourceRays(unit_ray, cone_angle, n_rays);

	return src;
}

std::vector<Ray> createSourceRays(Ray unit_ray, float cone_angle, size_t n_rays) {
	// Create arrays for all samples
	std::vector<Ray> rays;
	auto dir = unit_ray.dir;
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

void writeTriangleToFile(Triangle tri, std::ofstream &os) {
	os << tri.v1.x() << " " << tri.v1.y() << " " << tri.v1.z() << "\n";
	os << tri.v2.x() << " " << tri.v2.y() << " " << tri.v2.z() << "\n";
	os << "\n";
	os << tri.v3.x() << " " << tri.v3.y() << " " << tri.v3.z() << "\n";
	os << tri.v3.x() << " " << tri.v3.y() << " " << tri.v3.z() << "\n";
	os << "\n\n";
}

void writeRayToFile(Ray ray, std::ofstream &os) {
	float k = 10;
	Eigen::Vector3f endpoint = ray.pos + (k * ray.dir);
	os << ray.pos.x() << " " << ray.pos.y() << " " << ray.pos.z() << "\n";
	os << endpoint.x() << " " << endpoint.y() << " " << endpoint.z() << "\n";
	os << "\n\n";
}

void saveGeometryGNUPlot(const char *filename, Geometry geom) {
	std::ofstream gnu(filename);

	for (auto tri : geom.triangles) {
		writeTriangleToFile(tri, gnu);
	}

	gnu.close();
}

void saveSetupGNUPlot(const char *filename, Geometry geom, std::vector<Source> sources) {
	std::ofstream gnu(filename);

	// Script header
	gnu << "set view equal xyz\n";
	gnu << "splot \"-\" with lines, \"-\" lc rgb \"red\" lt 0.1 with lines\n";

	for (auto tri : geom.triangles) {
		writeTriangleToFile(tri, gnu);
	}

	gnu << "E\n";


	for (size_t i = 0; i < sources.size(); i++) {
		size_t ray_inc = 1;
		if (sources[i].rays.size() > 1E3) {
			auto k = sources[i].rays.size();
			std::cout << "Limiter " << k << "\n";

			ray_inc	= k / 1E3;
		}

		std::cout << "Ray inc " << ray_inc << "\n";

		for (size_t j = 0; j < sources[i].rays.size(); j += ray_inc) {
			writeRayToFile(sources[i].rays[j], gnu);
		}
	}

	gnu << "E\npause mouse close\n";

	gnu.close();
}
