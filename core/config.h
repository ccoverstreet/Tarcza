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
	model_data.geometry = parseObjFile(assembly_file.c_str());

	for (size_t i = 0; i < source_defs.size(); i++) {
		std::cout << source_defs[i]["coneAngle"] << "\n";
	}

	return model_data;
}
