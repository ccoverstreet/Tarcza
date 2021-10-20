#pragma once

#include <map>
#include <string>
#include <vector>

struct CrossSectionData {
	float E; // Energy in MeV
	float Sig_t; // Total attenuation cross section
	float Sig_pe; // photoelectric macroscopic cross section
};

std::map<std::string, std::vector<CrossSectionData>> m = {
ATTEN_DATA
};
