# Tarcza: main.py
# Cale Overstreet
# Oct. 14, 2021

# Tarcza is intended to allow for shielding calculations to be done with geometry files exported from CAD programs. This project is my current side-project and if it works, it will be used in my senior design. The concept is to use ray-tracing from a source (point-source intially) and find the frac of flux in each ray that makes it to a desired region. Using a uniform spherical distribution of rays, the fraction of the flux that makes it to a detector or tally region can be calculated.

import matplotlib.pyplot as plt
import numpy as np
import matplotlib.pyplot as plt

from objparser import parser
from tracer import tracer


def plotTriangles(triangles):
    fig = plt.figure()
    ax = fig.add_subplot(projection="3d")

    for tri in triangles[:]:
        ax.plot(np.append(tri[:-1, 0], tri[0, 0]),
                np.append(tri[:-1, 1], tri[0, 1]),
                np.append(tri[:-1, 2], tri[0, 2]), "-")

    plt.show()

# Much of Tarcza is done as np.array to facilitate using Numba
# Makes it look more C/C++ like
def main():
    print("Tarcza - A 3D OBJ-file based gamma flux tracer")
    print(parser)

    import time

    triangles, parts = parser.parseFile("../assembly_1.obj")
    ray_positions, ray_directions = tracer.createRays(int(1E5), [0, 0, 0], np.pi, [0, 1, 0])

    plotTriangles(triangles)

    start = time.time()
    tracer.traceGeometry(triangles, ray_positions,
            ray_directions)
    print("{} s".format(time.time() - start))


if __name__ == "__main__":
    main()
