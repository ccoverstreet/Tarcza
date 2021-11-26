#include "./types.h"
#include <cmath>
#include <iostream>
#include <fstream>

// --------- Printing overloads ---------
std::ostream &operator<<(std::ostream &output, const Triangle &t) {
	output << "\nTriangle\n";
	output << "\nv1\n " << t.v1 <<"\n"; 
	output << "v2\n" << t.v2 << "\n"; 
	output << "v3\n" << t.v3 << "\n"; 
	output << "norm\n" << t.norm << "\n"; 
	return output;
}

std::ostream &operator<<(std::ostream &output, const Part &p) {
	output << "\nPart\n";
	output << "\"" << p.name << "\" " << p.start << " " << p.end; 
	return output;
}

std::ostream &operator<<(std::ostream &output, const Ray &r ) {
	output << "\nRay\n";
	output << r.pos << "\n" << r.dir << "\n";
	return output;
}

std::ostream &operator<<(std::ostream &output, const Source &s ) {
	output << "\nSource\n";
	output << "Unit ray:\n"  << s.unit_ray.pos << "\n" << s.unit_ray.dir << "\n";
	output << "Cone angle (radians): " << s.cone_angle << "\n";
	output << "Number of rays: " << s.n_rays << "\n";
	output << "Emission area: " << s.area << "\n";
	return output;
}

// --------- END Printing overloads ---------

Part::Part(std::string in_name, size_t in_start, size_t in_end, YAML::Node mat) {
	name = in_name;
	start = in_start;
	end = in_end;
	material = mat;
}

std::string Geometry::getMaterial(size_t index) {
	for (auto partpair : parts) {
		if (index >= partpair.second.start && index < partpair.second.end) {
			return partpair.second.material["name"].as<std::string>();
		}
	}

	return "";
}

Ray::Ray(float x, float y, float z, float nx, float ny, float nz) {
	pos = Eigen::Vector3f(x, y, z);
	auto tmp = Eigen::Vector3f(nx, ny, nz);
	dir = tmp / sqrt(tmp.dot(tmp));
}

Ray::Ray(Eigen::Vector3f p, Eigen::Vector3f d) {
	pos = p;
	dir = d / sqrt(d.dot(d));
}

Ray::Ray(const Ray &ray) {
	pos = ray.pos;
	dir = ray.dir;
}

Ray::Ray() {
	pos = Eigen::Vector3f(0, 0, 0);
	dir = Eigen::Vector3f(0, 0, 0);
}

// Returns emission area (unit sphere)
float coneAngleToArea(float cone_angle) {
	return 2*float(M_PI)*(1 - cos(cone_angle/2));
}

Source::Source(Ray in_unit_ray, float in_cone_angle, size_t in_n_rays, float in_energy) {
	cone_angle = in_cone_angle;	
	n_rays = in_n_rays;	
	energy = in_energy;	
	unit_ray = in_unit_ray;
	area = coneAngleToArea(cone_angle);

	auto test_vec = Eigen::Vector3f(1, 0, 0);

	// Change comparison vector if too close to <1, 0, 0>
	if (abs(test_vec.dot(in_unit_ray.dir)) > 0.5) {
		test_vec = Eigen::Vector3f(0, 1, 0);
	}

	// Generate fake axes
	auto norm1 = in_unit_ray.dir.cross(test_vec);
	auto j = in_unit_ray.dir.cross(norm1);
	auto k = in_unit_ray.dir.cross(j);

	// Normalize fake axes
	j = j / sqrt(j.dot(j));
	k = k / sqrt(k.dot(k));

	Eigen::Matrix<float, 3, 3> R {
		{in_unit_ray.dir.x(), j.x(), k.x()},
		{in_unit_ray.dir.y(), j.y(), k.y()},
		{in_unit_ray.dir.z(), j.z(), k.z()}
	};

	rot_matrix = R;
}

Ray Source::generateRay(std::mt19937 &gen) {
	float d = sqrt(m_dist(gen));
	
	float phi = cone_angle / 2;
	float bot = cos(phi);

	float probe = sqrt(m_dist(gen)) * (1-bot) + bot;
	float theta = m_dist(gen) * 2 * M_PI;

	float s_phi = acos(probe);

	if (n_rays == 1) {
		return unit_ray;
	}

	float x = cos(s_phi);
	float y = sin(s_phi) * cos(theta);
	float z = sin(s_phi) * sin(theta);

	Eigen::Vector3f v(x, y, z);
	Eigen::Vector3f res = rot_matrix * v;
	return Ray(unit_ray.pos, res);
}

void writeTriangleToFile(Triangle tri, std::ofstream &os) {
	os << tri.v1.x() << " " << tri.v1.y() << " " << tri.v1.z() << "\n";
	os << tri.v2.x() << " " << tri.v2.y() << " " << tri.v2.z() << "\n";
	os << "\n";
	os << tri.v3.x() << " " << tri.v3.y() << " " << tri.v3.z() << "\n";
	os << tri.v3.x() << " " << tri.v3.y() << " " << tri.v3.z() << "\n";
	os << "\n\n";
}

void writeRayToFile(Ray ray, std::ofstream &os, float scaling) {
	float k = scaling; // Arbitrary scaling value
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

void saveSetupGNUPlot(const char *filename, Geometry geom, std::vector<Source> &sources) {
	std::ofstream gnu(filename);

	// Script header
	gnu << "set view equal xyz\n";
	gnu << "splot \"-\" with lines, \"-\" lc rgb \"red\" lt 0.01 with lines\n";

	for (auto tri : geom.triangles) {
		writeTriangleToFile(tri, gnu);
	}

	gnu << "E\n";

	std::mt19937 rng(0);

	for (size_t i = 0; i < sources.size(); i++) {
		size_t ray_inc = 1;
		if (sources[i].n_rays > 1E3) {
			auto k = sources[i].n_rays;

			ray_inc	= k / 1E3;
		}

		auto test_ray = sources[i].generateRay(rng).pos;
		float scaling = abs(sqrt(test_ray.dot(test_ray)));

		for (size_t j = 0; j < sources[i].n_rays; j += ray_inc) {
			writeRayToFile(sources[i].generateRay(rng), gnu, scaling);
		}
	}

	gnu << "E\npause mouse close\n";

	gnu.close();
}
