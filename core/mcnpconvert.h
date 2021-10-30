#include "./types.h"
#include "./config.h"
#include <fstream>
#include <sstream>

void writeSurfaces(const char *filename, Geometry geometry) {
	std::ofstream mcnpfile(filename);

	std::stringstream ss_surfaces;
	std::stringstream ss_cells;
	std::stringstream ss_container;

	ss_cells << "c ---------- Cells ---------\n";
	ss_surfaces << "c ---------- Surfaces ---------\n";

	ss_container << "998 0 -999 ";
	
	size_t p_index = 1;
	size_t cur_cell = 1;
	for (auto part : geometry.parts) {
		ss_cells << cur_cell << " 0 ";
		ss_container << "(";
		for (size_t i = part.second.start; i < part.second.end; i++) {
			if (i != part.second.start) {
				ss_container << ":";
			}
			if (i % 10 == 0) {
				ss_cells << "\n     ";
				ss_container << "\n     ";
			}
			auto tri = geometry.triangles[i];
			auto D = tri.norm.dot(tri.v1);


			ss_surfaces << p_index << " p " << tri.norm.x() << " " << tri.norm.y() << " " << tri.norm.z() << " " << D << "\n";
			ss_cells << "-" << p_index << " ";
			ss_container << p_index;
			
			p_index++;
		}

		ss_container << ") ";

		ss_cells << "\n     imp:p=1\n";

		cur_cell++;
	}

	ss_container << "imp:p=1\n";
	
	ss_cells << ss_container.str();
	ss_cells << "999 0 999 imp:p=0\n";
	ss_cells << "c ---------- END Cells ---------\n";
	ss_surfaces << "999 SO 300\n";
	ss_surfaces << "c ---------- END Surfaces ---------\n";

	mcnpfile << ss_cells.str() << "\n" << ss_surfaces.str();

	mcnpfile.close();
}

