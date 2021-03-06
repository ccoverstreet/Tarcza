#include <iostream>
#include <vector>
#include <fstream>

#include <omp.h>
#include <yaml-cpp/yaml.h>

#include "./objparser.h"
#include "./tracer.h"
#include "./config.h"
#include "./mcnpconvert.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Usage: Tarcza <inputfilename>\n";
		return 0;
	}

	TarczaConfig config = createTarczaConfig(argv[1]);

	printf("\nGeometry:\n");
	for (auto part : config.geometry.parts) {
		std::cout << part.second.name << "\n";
		std::cout << part.second.start << "\n";
		std::cout << part.second.end << "\n";
		std::cout << part.second.material << "\n";
		printf("\n");
	}

	//writeSurfaces("mcnpsurfaces.txt", config.geometry);

	printf("# of parts: %d\n", config.geometry.parts.size());
	printf("# of triangles: %d\n", config.geometry.triangles.size());

	std::cout << "Saving setup gnuplot script\n";
	saveSetupGNUPlot("setup.gnuplot", config.geometry, config.sources);

	tarczaTracingRoutine(config.geometry, config.sources);

	return 0;
}
