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
	{"Pb", {
			{0.001, 5197.0, 5209.0},
			{0.0015, 2344.0, 2356.0},
			{0.002, 1274.0, 1285.0},
			{0.002484, 790.0, 800.9},
			{0.002484, 1385.0, 1396.0},
			{0.002534, 1636.0, 1647.0},
			{0.002586, 1933.0, 1944.0},
			{0.002586, 2439.0, 2450.0},
			{0.003, 1955.0, 1965.0},
			{0.003066, 1847.0, 1857.0},
			{0.003066, 2136.0, 2146.0},
			{0.003301, 1782.0, 1792.0},
			{0.003554, 1486.0, 1496.0},
			{0.003554, 1575.0, 1585.0},
			{0.003699, 1432.0, 1441.0},
			{0.003851, 1302.0, 1311.0},
			{0.003851, 1358.0, 1368.0},
			{0.004, 1242.0, 1251.0},
			{0.005, 722.2, 730.5},
			{0.006, 459.8, 467.2},
			{0.008, 222.6, 228.7},
			{0.01, 125.6, 130.6},
			{0.01304, 63.1, 67.0},
			{0.01304, 158.2, 162.1},
			{0.015, 108.2, 111.6},
			{0.0152, 104.5, 107.8},
			{0.0152, 145.2, 148.5},
			{0.01553, 138.0, 141.2},
			{0.01586, 131.2, 134.4},
			{0.01586, 151.7, 154.8},
			{0.02, 83.97, 86.37},
			{0.03, 28.86, 30.32},
			{0.04, 13.35, 14.36},
			{0.05, 7.292, 8.042},
			{0.06, 4.432, 5.02},
			{0.08, 2.012, 2.419},
			{0.088, 1.547, 1.91},
			{0.088, 7.321, 7.684},
			{0.1, 5.237, 5.549},
			{0.15, 1.815, 2.015},
			{0.2, 0.8464, 0.9986},
			{0.3, 0.293, 0.4032},
			{0.4, 0.1417, 0.2323},
			{0.5, 0.08257, 0.1613},
			{0.6, 0.05406, 0.1248},
			{0.8, 0.02871, 0.0887},
			{1.0, 0.0181, 0.07102},
			{1.022, 0.01732, 0.06962},
			{1.25, 0.01168, 0.05875},
			{1.5, 0.008321, 0.05222},
			{2.0, 0.005034, 0.04607}
		}
	},
	{"Al", {
			{0.001, 1183.0, 1185.0},
			{0.0015, 400.2, 402.3},
			{0.00156, 360.0, 362.1},
			{0.00156, 3955.0, 3957.0},
			{0.002, 2261.0, 2263.0},
			{0.003, 786.5, 788.1},
			{0.004, 359.1, 360.5},
			{0.005, 192.2, 193.4},
			{0.006, 114.3, 115.3},
			{0.008, 49.5, 50.32},
			{0.01, 25.56, 26.21},
			{0.015, 7.515, 7.955},
			{0.02, 3.1, 3.442},
			{0.03, 0.8722, 1.128},
			{0.04, 0.3504, 0.5684},
			{0.05, 0.1718, 0.3681},
			{0.06, 0.09564, 0.2778},
			{0.08, 0.03783, 0.2018},
			{0.1, 0.0184, 0.1704},
			{0.15, 0.004993, 0.1378},
			{0.2, 0.002002, 0.1223},
			{0.3, 0.0005743, 0.1042},
			{0.4, 0.000248, 0.09276},
			{0.5, 0.0001344, 0.08445},
			{0.6, 8.401e-05, 0.07802},
			{0.8, 4.252e-05, 0.06841},
			{1.0, 2.643e-05, 0.06146},
			{1.022, 2.489e-05, 0.0608},
			{1.25, 1.688e-05, 0.05496},
			{1.5, 1.222e-05, 0.05006},
			{2.0, 7.633e-06, 0.04324}
		}
	},
	{"Ge", {
			{0.001, 1887.0, 1892.0},
			{0.001103, 1495.0, 1501.0},
			{0.001217, 1185.0, 1190.0},
			{0.001217, 4356.0, 4361.0},
			{0.001232, 4651.0, 4656.0},
			{0.001248, 4966.0, 4972.0},
			{0.001248, 6650.0, 6655.0},
			{0.001328, 6074.0, 6079.0},
			{0.001414, 5549.0, 5554.0},
			{0.001414, 6282.0, 6287.0},
			{0.0015, 5470.0, 5475.0},
			{0.002, 2706.0, 2711.0},
			{0.003, 957.1, 961.2},
			{0.004, 446.1, 449.7},
			{0.005, 244.1, 247.3},
			{0.006, 148.1, 150.9},
			{0.008, 66.75, 68.9},
			{0.01, 35.71, 37.42},
			{0.0111, 26.59, 28.11},
			{0.0111, 196.6, 198.2},
			{0.015, 90.4, 91.48},
			{0.02, 41.44, 42.22},
			{0.03, 13.35, 13.85},
			{0.04, 5.843, 6.206},
			{0.05, 3.044, 3.335},
			{0.06, 1.775, 2.023},
			{0.08, 0.7515, 0.9501},
			{0.1, 0.3835, 0.555},
			{0.15, 0.1125, 0.2491},
			{0.2, 0.04732, 0.1661},
			{0.3, 0.01437, 0.1131},
			{0.4, 0.006412, 0.09327},
			{0.5, 0.003541, 0.08212},
			{0.6, 0.002238, 0.07452},
			{0.8, 0.001144, 0.06426},
			{1.0, 0.000712, 0.05727},
			{1.022, 0.0006766, 0.05662},
			{1.25, 0.0004572, 0.05101},
			{1.5, 0.0003291, 0.04657},
			{2.0, 0.0002021, 0.04086}
		}
	}

};
