#pragma once

#include <Eigen/Dense>
#include <cstdint>
#include <map>
#include <vector>

struct Triangle {
	Eigen::Vector3f v1;
	Eigen::Vector3f v2;
	Eigen::Vector3f v3;
	Eigen::Vector3f norm;

	friend std::ostream &operator<<(std::ostream &output, const Triangle &t); 
};

struct Part {
	std::string name;
	size_t start;
	size_t end;
	std::string material;

	Part(std::string in_name, size_t in_start, size_t in_end, std::string mat);

	friend std::ostream &operator<<(std::ostream &output, const Part &p);
};

struct Geometry {
	std::vector<Triangle> triangles;
	std::map<std::string, Part> parts;

	std::string getMaterial(size_t index); // returns material string for given part
};

struct Ray {
	Eigen::Vector3f pos;
	Eigen::Vector3f dir;

	Ray(float x, float y, float z, float nx, float ny, float nz);
	Ray(Eigen::Vector3f p, Eigen::Vector3f d);
	Ray(const Ray &ray);
	Ray();

	friend std::ostream &operator<<(std::ostream &output, const Ray &r);
};

struct Source {
	Ray unit_ray; // Contains the pos and dir that rays are generated from
	float cone_angle;
	size_t n_rays;
	float energy;
	float area; // Emission area

	Eigen::Matrix<float, 3, 3> rot_matrix;

	Source(Ray unit_ray, float cone_angle, size_t n_rays, float energy);

	Ray generateRay(size_t ray_n);

	friend std::ostream &operator<<(std::ostream &output, const Source &s);
};


void saveGeometryGNUPlot(const char *filename, Geometry geom);
void saveSetupGNUPlot(const char *filename, Geometry geom, std::vector<Source> &sources);

std::vector<Ray> createSourceRays(Ray unit_ray, float cone_angle, size_t n_rays);

