#include "./tracer.h"

#include <omp.h>
#include <map>


void tarczaTracingRoutine(Geometry geom, std::vector<Source> sources) {
	printf("\nTarcza Tracing Routine\n");
	auto start = omp_get_wtime();
	printf("Start at: %d\n", start);

	for (size_t src_n = 0; src_n < sources.size(); src_n++ ) {
		auto src = sources[src_n];
		std::map<std::string, float> cross_section_map;
		cross_section_map.insert(std::pair<std::string, float>("Pb", 1E-3));
		#pragma omp parallel for
		for (size_t ray_n = 0; ray_n < src.rays.size(); ray_n++) {
			traceRayPath(geom, src.rays[ray_n], cross_section_map);
		}
	}

	auto end = omp_get_wtime();
	printf("End at: %d\n", end);

	printf("Time taken to trace rays: %f \n", (end - start));
}

void traceRayPath(Geometry geom, Ray ray, std::map<std::string, float> cross_sections) {					
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

		auto x = tri.norm.dot(ray.dir) * tri.v1;
		auto y = x.dot(ray.dir);
		sum += y;
		count += 1;
	}
}
