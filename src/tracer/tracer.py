# Main calculation file
# For each ray, iterates through all triangles and determines the index of the intersected triangles, position of intersection along ray, and material for each region marked by separation on the ray

from dataclasses import dataclass
import numpy as np
import numba

from matmath import matmath

@dataclass
class Ray:
    pos: np.array
    direction: np.array

def createRays(n_rays, pos, angle, direction):
        dirs = matmath.fibonacciSphere(n_rays, angle, direction)
        poss = np.zeros(shape=np.shape(dirs))
        for i in range(len(poss)):
            poss[i, 0] = pos[0]
            poss[i, 1] = pos[1]
            poss[i, 2] = pos[2]

        return poss, np.array(dirs)

class Source:
    def __init__(self, n_rays, pos, angle, direction):
        self.n_rays = n_rays
        self.angle = angle
        self.dir = direction

        self.rays = [Ray(np.array([pos[0], pos[1], pos[2]]), np.array([d[0], d[1], d[2]])) for d in matmath.fibonacciSphere(n_rays, angle, direction)]
        # Need to generate array of rays
        # Evenly distributed across across conical section of sphere



@numba.jit(nopython=True)
def traceGeometry(triangles, ray_poss, ray_dirs):
    #print("\n\nTarcza Ray Tracing Routine")
    #print("{:<15} {}".format("# of triangles:", len(triangles)))
    ##print("{:<15} {}".format("# of rays:", len(rays)))
    #print("{:<15} {}".format("# of parts:", len(parts)))
    #print("Parts:")
    #for part in parts:
    #    print("\t{}: Index {} -> {}".format(part, parts[part][0], parts[part][1]))

    for s_n in range(len(ray_poss)):
        for t_n in numba.prange(len(triangles)):
            np.dot(ray_poss[s_n], triangles[t_n][3]) > 0.5
                
    #print("Number of calculations: {}".format(count))


