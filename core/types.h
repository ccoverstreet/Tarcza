#pragma once

#include <Eigen/Dense>

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

	friend std::ostream &operator<<(std::ostream &output, const Ray &r ) {
		output << r.pos << "\n" << r.dir << "\n";
		return output;
	}
};
