#include <iostream>
#include <vector>
#include <fstream>

#include <omp.h>
#include <yaml-cpp/yaml.h>

#include "./core/objparser.h"
#include "./core/tracer.h"
#include "./core/config.h"
#include "./core/mcnpconvert.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Usage: Tarcza <inputfilename>\n";
		return 0;
	}

	TarczaConfig config = createTarczaConfig(argv[1]);

	for (auto part : config.geometry.parts) {
		std::cout << part.second.name << "\n";
		std::cout << part.second.start << "\n";
		std::cout << part.second.end << "\n";
		std::cout << part.second.material << "\n";
	}

	writeSurfaces("mcnpsurfaces.txt", config.geometry);

	printf("# of parts: %d\n", config.geometry.parts.size());
	printf("# of triangles: %d\n", config.geometry.triangles.size());

	std::cout << "Saving setup gnuplot script\n";
	saveSetupGNUPlot("setup.gnuplot", config.geometry, config.sources);

	tarczaTracingRoutine(config.geometry, config.sources);

	return 0;
}
