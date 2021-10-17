#include "./objparser.h"

#include <string>
#include <iostream>
#include <cstdint>
#include <cmath>

Geometry parseObjFile(const char *filename) {
	std::fstream input_file(filename);

	std::vector<Eigen::Vector3f> vertices;
	std::vector<Triangle> triangles;
	std::vector<Part> parts;

	std::string cur_part = "";
		
	uint32_t cur_tri_index = 0;
	uint32_t start_tri_index = 0;

	while (!input_file.fail()) {
		std::string desc;
		input_file >> desc;
		if (desc == "o") {
			continue;
		} else if (desc == "g") {
			while (input_file.peek() == ' ') {
				input_file.ignore(1);
			}

			if (cur_part != "") {
				parts.push_back(Part{cur_part, start_tri_index, cur_tri_index});
				start_tri_index = cur_tri_index;
			}

			std::string part;
			std::getline(input_file, part);
			std::cout << "Reading part \"" << part << "\"...\n";
			cur_part = part;
		} else if (desc == "v") {
			float x, y, z;
			input_file >> x;
			input_file >> y;
			input_file >> z;

			vertices.push_back(Eigen::Vector3f(x, y, z) * 100);
		} else if (desc == "f") {
			std::string v1;
			std::string v2;
			std::string v3;

			input_file >> v1 >> v2 >> v3;

			uint32_t i_v1 = std::stoi(v1.substr(0, v1.find("/"))) - 1;
			uint32_t i_v2 = std::stoi(v2.substr(0, v2.find("/"))) - 1;
			uint32_t i_v3 = std::stoi(v3.substr(0, v3.find("/"))) - 1;

			auto vect1 = vertices[i_v1];
			auto vect2 = vertices[i_v2];
			auto vect3 = vertices[i_v3];

			auto raw_norm = (vect2 - vect1).cross(vect3 - vect1);
			auto norm = raw_norm / sqrt(raw_norm.dot(raw_norm)); //+ 1E-10f);
			//std::cout << norm << "\n\n";

			triangles.push_back(Triangle{vect1, vect2, vect3, norm});
		}
	}

	parts.push_back(Part{cur_part, start_tri_index, cur_tri_index});
	start_tri_index = cur_tri_index;

	//printVector(vertices);
	//printVector(parts);
	//printVector(triangles);

	printf("# of vertices: %d\n", vertices.size());
	printf("# of triangles: %d\n", triangles.size());

	return Geometry{triangles, parts};
}
