#include "./tracer.h"

#include <omp.h>

void tarczaTracingRoutine(Geometry geom, std::vector<Source> sources) {
	printf("\nTarcza Tracing Routine\n");
	auto start = omp_get_wtime();
	printf("Start at: %d\n", start);

	for (auto src : sources) {
		#pragma omp parallel for
		for (auto ray : src.rays) {
			traceRayPath(geom, ray);
		}
	}

	auto end = omp_get_wtime();
	printf("End at: %d\n", end);

	printf("Time taken to trace rays: %f \n", (end - start));
	//printf("Time per ray: %f\n", (end-start) / float(rays.size()));
}

void traceRayPath(Geometry geom, Ray ray) {					
	uint32_t count = 0;
	double sum = 0;

	for (auto tri : geom.triangles)	{
		auto x = tri.norm.dot(ray.dir) * tri.v1;
		auto y = x.dot(ray.dir);
		sum += y;
		count += 1;
	}
}
