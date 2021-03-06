#include "./tracer.h"

#include <omp.h>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <chrono>
#include "../attenuation/coefficients.h"


float calculateTransmission(float distance, float total_attenuation_coeff) {
	return exp(-1 * distance * total_attenuation_coeff);
}

float linearInterpolate(float x, float x1, float y1, float x2, float y2) {
	return (y2 - y1) / (x2 - x1) * (x - x1) + y1;
}

float logLogLinearInterpolate(float x, float x1, float y1, float x2, float y2) {
	double lx = log10(x);
	double lx1 = log10(x1);
	double ly1 = log10(y1);
	double lx2 = log10(x2);
	double ly2 = log10(y2);

	return pow(10, (ly2 - ly1) / (lx2 - lx1) * (lx - lx1) + ly1);
}

AttenCoeff calculateAttenuationCoeffs(Part part, float energy) {
	auto coeffData = COEFF_DATABASE[part.material["name"].as<std::string>()];
	auto density  = part.material["density"].as<float>();

	size_t lower_index = 0;
	for (size_t i = 0; i < coeffData.size() - 1; i++) {
		if (energy < coeffData[i].E) {
			lower_index = i - 1;
			break;
		}
	}

	auto total_atten = logLogLinearInterpolate(energy, coeffData[lower_index].E, coeffData[lower_index].Sig_t, coeffData[lower_index + 1].E, coeffData[lower_index + 1].Sig_t);
	auto pe_atten = logLogLinearInterpolate(energy, coeffData[lower_index].E, coeffData[lower_index].Sig_pe, coeffData[lower_index + 1].E, coeffData[lower_index + 1].Sig_pe);

	return AttenCoeff{total_atten * density, pe_atten * density};
}

std::map<std::string, AttenCoeff> createCoeffMapForEnergy(float energy, std::map<std::string, Part> &parts) {
	std::map<std::string, AttenCoeff> output_map;
	for (auto pair : parts) {
		if (pair.second.material["name"].as<std::string>() == "VOID") continue;
		output_map.insert(std::pair<std::string, AttenCoeff>(pair.second.material["name"].as<std::string>(), calculateAttenuationCoeffs(pair.second, energy)));
	}

	return output_map;
}

void tarczaTracingRoutine(Geometry geom, std::vector<Source> sources) {
	printf("\nTarcza Tracing Routine\n");
	auto start = omp_get_wtime();
	printf("Start at: %d\n", start);



	// Setup RNG
	auto now = std::chrono::high_resolution_clock::now();
	auto s_seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	srand(s_seed);
	std::vector<std::mt19937> rngs;
	for (size_t i = 0; i < omp_get_max_threads(); i++) {
		rngs.push_back(std::mt19937(rand()));
	}

	size_t ray_count = 0;
	for (size_t src_n = 0; src_n < sources.size(); src_n++ ) {
		ray_count += sources[src_n].n_rays;
		auto src = sources[src_n];
		auto coeff_map = createCoeffMapForEnergy(sources[src_n].energy, geom.parts);

		float contribution_sum = 0;
		float area_per_ray = sources[src_n].area / float(sources[src_n].n_rays) / (4 * M_PI) / 2;
		std::cout << "Area per ray: " << area_per_ray << "\n";
		#pragma omp parallel for reduction(+: contribution_sum)
		for (size_t ray_n = 0; ray_n < src.n_rays; ray_n++) {
			contribution_sum += area_per_ray * traceRayPath(geom, src.generateRay(rngs[omp_get_thread_num()]), coeff_map);
		}

		contribution_sum = contribution_sum;

		std::cout << "Contribution sum: " << contribution_sum << "\n";
	}

	auto end = omp_get_wtime();
	printf("End at: %d\n", end);

	printf("Time taken to trace rays: %f s\n", (end - start));
	printf("Time per ray: %f s\n", (end - start) / float(ray_count));
}


// TODO: Replace comparison with legitimate float compare
bool sortCollision(const Collision &a, const Collision &b) {
	if (a.t_collision == b.t_collision) {
		return a.dir_dot_norm > b.dir_dot_norm;
	}

	return a.t_collision < b.t_collision;
}

// returns contribution of ray flux
float traceRayPath(Geometry geom, Ray ray, std::map<std::string, AttenCoeff> &coeffs) {
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

		if (t_intersection < 0) {
			continue;
		}

		// Position of intersection in 3D space
		auto pos_int = ray.pos + t_intersection * ray.dir;

		float check_1 = (tri.v2 - tri.v1).cross(pos_int - tri.v1).dot(tri.norm);
		float check_2 = (tri.v3 - tri.v2).cross(pos_int - tri.v2).dot(tri.norm);
		float check_3 = (tri.v1 - tri.v3).cross(pos_int - tri.v3).dot(tri.norm);

		bool is_inside = check_1 > 0 && check_2 >= 0 && check_3 >= 0;


		if (!is_inside) {
			continue;
		}

		collisions.push_back(Collision{t_intersection, n_dot_d, geom.getMaterial(i)});
	}


	std::sort(collisions.begin(), collisions.end(), sortCollision);

	return calculateRayContribution(collisions, coeffs);
}



float calculateRayContribution(std::vector<Collision> &collisions, std::map<std::string, AttenCoeff> &coeffs) {
	float coeff_air = 0; // Assuming near 0 attenuation in air

	float cur_flux = 1.0;
	float cur_position = 0; // Current position along ray
	std::string cur_material = "xx"; // "xx" is used to define void
	float cur_contribution = 0.0;

	for (auto collision : collisions) {
		float coeff = coeff_air; //
		if (collision.material == cur_material) {
			coeff = coeffs[collision.material].Tot;
		}

		float thickness = collision.t_collision - cur_position;
		float new_flux = cur_flux * calculateTransmission(thickness, coeff);

		if (collision.material == "Ge") {
			cur_contribution += coeffs[collision.material].PE / coeffs[collision.material].Tot * (cur_flux - new_flux);
		}

		// Updating values for loop iteration
		cur_material = collision.material;
		cur_position = collision.t_collision;
		cur_flux = new_flux;
	}

	return cur_contribution;
}
