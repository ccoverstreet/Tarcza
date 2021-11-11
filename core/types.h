#pragma once

#include <Eigen/Dense>
#include <cstdint>
#include <map>
#include <vector>
#include <random>
#include <yaml-cpp/yaml.h>

struct Triangle {
	Eigen::Vector3f v1;
	Eigen::Vector3f v2;
	Eigen::Vector3f v3;
	Eigen::Vector3f norm;

	friend std::ostream &operator<<(std::ostream &output, const Triangle &t); 
};

struct Material {
	float density;
};

struct Part {
	std::string name;
	size_t start;
	size_t end;
	YAML::Node material;

	Part(std::string in_name, size_t in_start, size_t in_end, YAML::Node mat);

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
	std::uniform_real_distribution<float> m_dist = std::uniform_real_distribution<float>(0.0, 1.0);

	Source(Ray unit_ray, float cone_angle, size_t n_rays, float energy);

	Ray generateRay(std::mt19937 &gen);

	friend std::ostream &operator<<(std::ostream &output, const Source &s);
};


void saveGeometryGNUPlot(const char *filename, Geometry geom);
void saveSetupGNUPlot(const char *filename, Geometry geom, std::vector<Source> &sources);


