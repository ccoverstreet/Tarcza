#pragma once

#include <map>
#include <string>
#include <vector>

struct CrossSectionData {
	float E; // Energy in MeV
	float Sig_pe; // photoelectric macroscopic cross section
	float Sig_t; // Total attenuation cross section
};

std::map<std::string, std::vector<CrossSectionData>> COEFF_DATABASE = {
ATTEN_DATA
};
