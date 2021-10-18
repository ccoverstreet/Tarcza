// Tarcza: Configuration definitions

#pragma once

#include <string>

#include <yaml-cpp/yaml.h>

#include "./types.h"
#include "./objparser.h"


struct TarczaConfig {
	Geometry geometry;		
	std::vector<Source> sources;
};

TarczaConfig createTarczaConfig(const char *filename) {
	TarczaConfig model_data;

	YAML::Node config = YAML::LoadFile(filename);

	YAML::Node source_defs = config["sources"];
	std::string assembly_file = config["assemblyFile"].as<std::string>();
	YAML::Node partname_map = config["partNameMap"];
	YAML::Node material_map = config["materialMap"];

	model_data.geometry = parseObjFile(assembly_file.c_str(), partname_map);

	// Parse source definitions
	std::vector<Source> sources;

	std::cout << "Reading source definitions...\n";
	for (auto source_def : source_defs) {
		std::cout << "Source found.\n";
		std::vector<float> unit_ray_elems = source_def["unitRay"].as<std::vector<float>>();
		float cone_angle = source_def["coneAngle"].as<float>();
		float energy = source_def["energy"].as<float>();
		size_t n_rays = source_def["numberOfRays"].as<size_t>();

		std::cout << "Cone angle: " << cone_angle << "\n";
		std::cout << "Number of rays: " << n_rays << "\n";

		std::cout << unit_ray_elems.size() << "\n";
		auto unit_ray = Ray(unit_ray_elems[0],
				unit_ray_elems[1],
				unit_ray_elems[2],
				unit_ray_elems[3],
				unit_ray_elems[4],
				unit_ray_elems[5]);

		std::cout << unit_ray;

		sources.push_back(createSource(unit_ray, cone_angle, n_rays, energy));
	}

	model_data.sources = sources;

	return model_data;
}
