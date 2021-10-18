#pragma once

#include <Eigen/Dense>
#include <cstdint>
#include <vector>

struct Triangle {
	Eigen::Vector3f v1;
	Eigen::Vector3f v2;
	Eigen::Vector3f v3;
	Eigen::Vector3f norm;

	friend std::ostream &operator<<(std::ostream &output, const Triangle &t ) {
		output << "\nv1\n " << t.v1 <<"\n"; 
		output << "v2\n" << t.v2 << "\n"; 
		output << "v3\n" << t.v3 << "\n"; 
		output << "norm\n" << t.norm << "\n"; 
		return output;
	}
};

struct Part {
	std::string name;
	uint32_t start;
	uint32_t end;

	friend std::ostream &operator<<(std::ostream &output, const Part &p ) {
		output << "\"" << p.name << "\" " << p.start << " " << p.end; 
		return output;
	}
};

struct Geometry {
	std::vector<Triangle> triangles;
	std::vector<Part> parts;
};

struct Ray {
	Eigen::Vector3f pos;
	Eigen::Vector3f dir;

	Ray(float x, float y, float z, float nx, float ny, float nz) {
		pos = Eigen::Vector3f(x, y, z);
		auto tmp = Eigen::Vector3f(nx, ny, nz);
		dir = tmp / sqrt(tmp.dot(tmp));
	}

	Ray(Eigen::Vector3f p, Eigen::Vector3f d) {
		pos = p;
		dir = d / sqrt(d.dot(d));
	}

	friend std::ostream &operator<<(std::ostream &output, const Ray &r ) {
		output << r.pos << "\n" << r.dir << "\n";
		return output;
	}
};

struct Source {
	Ray unit_ray; // Contains the pos and dir that rays are generated from
	float cone_angle;
	size_t n_rays;
	float area; // Emission area

	std::vector<Ray> rays;

	friend std::ostream &operator<<(std::ostream &output, const Source &s ) {
		output << "Unit ray:\n"  << s.unit_ray.pos << "\n" << s.unit_ray.dir << "\n";
		output << "Cone angle (radians): " << s.cone_angle << "\n";
		output << "Number of rays: " << s.n_rays << "\n";
		output << "Emission area: " << s.area << "\n";
		return output;
	}
};


void saveGeometryGNUPlot(const char *filename, Geometry geom);
void saveSetupGNUPlot(const char *filename, Geometry geom, std::vector<Source> sources);

Source createSource(Ray unit_ray, float cone_angle, size_t n_rays);
std::vector<Ray> createSourceRays(Ray unit_ray, float cone_angle, size_t n_rays);

