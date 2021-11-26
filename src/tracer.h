// Tarcza: Tracing Routine
// Cale Overstreet
// Oct. 15, 2021
/* This routine is the primary calculation loop for Tarcza. 
 */

#pragma once

#include "./objparser.h"
#include "./types.h"
#include <map>
#include <cmath>

float calculateTransmission(float distance, float total_attenuation_coeff);

struct Collision {
	float t_collision;
	float dir_dot_norm;
	std::string material;
};

struct AttenCoeff {
	float Tot; // Total coefficient
	float PE; // Photoelectric coefficient
};

AttenCoeff calculateAttenuationCoeffs(Part part, float energy);

std::map<std::string, AttenCoeff> createCoeffMapForEnergy(float energy, std::map<std::string, Part> &parts);

void tarczaTracingRoutine(Geometry geom, std::vector<Source> rays);
float traceRayPath(Geometry geom, Ray ray, std::map<std::string, AttenCoeff> &cross_sections);

// Returns fraction of ray flux sensed in detector
float calculateRayContribution(std::vector<Collision> &collisions, std::map<std::string, AttenCoeff> &coeffs);

