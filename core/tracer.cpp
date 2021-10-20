#include "./tracer.h"

#include <omp.h>
#include <map>
#include <algorithm>
#include "../attenuation/coefficients.h"


void tarczaTracingRoutine(Geometry geom, std::vector<Source> sources) {
	printf("\nTarcza Tracing Routine\n");
	auto start = omp_get_wtime();
	printf("Start at: %d\n", start);

	for (size_t src_n = 0; src_n < sources.size(); src_n++ ) {
		auto src = sources[src_n];
		std::map<std::string, float> cross_section_map;
		cross_section_map.insert(std::pair<std::string, float>("Pb", 1E-3));
		#pragma omp parallel for
		for (size_t ray_n = 0; ray_n < src.n_rays; ray_n++) {
			traceRayPath(geom, src.generateRay(ray_n), cross_section_map);
		}
	}

	auto end = omp_get_wtime();
	printf("End at: %d\n", end);

	printf("Time taken to trace rays: %f \n", (end - start));
}

struct Collision {
	float t_collision;
	float dir_dot_norm;
	std::string material;
};


// TODO: Replace comparison with legitamate float compare
bool sortCollision(const Collision &a, const Collision &b) {
	if (a.t_collision == b.t_collision) {
		return a.dir_dot_norm > b.dir_dot_norm;
	}

	return a.t_collision < b.t_collision;
}

void traceRayPath(Geometry geom, Ray ray, std::map<std::string, float> cross_sections) {					
	std::vector<Collision> collisions;

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
		printf("Triangle %d: %d\n", i, is_inside);

		if (!is_inside) {
			continue;
		}

		collisions.push_back(Collision{t_intersection, n_dot_d, geom.getMaterial(i)});
	}

	std::sort(collisions.begin(), collisions.end(), sortCollision);

	for (auto col : collisions) {
		std::cout << "\n";
		std::cout << col.material << "\n";
		std::cout << col.t_collision << "\n";
		std::cout << col.dir_dot_norm << "\n";
	}
}
