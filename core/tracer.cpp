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

	for (size_t i = 0; i < geom.triangles.size(); i++) {
		auto tri = geom.triangles[i];
		auto n_dot_d = tri.norm.dot(ray.dir);

		// Check if intersection ever occurs
		if (n_dot_d == 0) {
			continue;
		} 

		auto n_plane_dot_pos = tri.norm.dot(tri.v1 - ray.pos);

		float t_intersection = n_plane_dot_pos / n_dot_d;

		// Position of intersection in 3D space
		auto pos_int = ray.pos + t_intersection * ray.dir;

		float check_1 = (tri.v2 - tri.v1).cross(pos_int - tri.v1).dot(tri.norm);
		float check_2 = (tri.v3 - tri.v2).cross(pos_int - tri.v2).dot(tri.norm);
		float check_3 = (tri.v1 - tri.v3).cross(pos_int - tri.v3).dot(tri.norm);

		bool is_inside = check_1 > 0 && check_2 >= 0 && check_3 >= 0;

		//printf("Checks: %f %f %f -> %d -> Pos: %f %f %f\n", 
		//		check_1, check_2, check_3, is_inside, pos_int[0], pos_int[1], pos_int[2]);
		
		auto x = tri.norm.dot(ray.dir) * tri.v1;
		auto y = x.dot(ray.dir);
		sum += y;
		count += 1;
	}
}
