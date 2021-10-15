from dataclasses import dataclass
import numpy as np

from matmath import matmath

@dataclass
class Vertex:
    x: float
    y: float
    z: float

class Triangle:
    a: Vertex
    b: Vertex
    c: Vertex

@dataclass(frozen=True)
class Part:
    start: int
    end: int


# This parsing scheme only works for OnShape's generated OBJ files
def parseFile(filename):
    parts = []
    vertices = []
    triangles = []
    current_part = ""

    # cur_tri_index is used to denote what triangles correspond to each face
    cur_tri_index = 0
    start_tri_index = 0
    with open(filename) as f:
        for line in f:
            if line.startswith(("o ")):
                # The "o" in object files seems to be an optional separator
                continue
            elif line.startswith("g "):
                if current_part != "":

                    parts.append((start_tri_index, cur_tri_index))
                    start_tri_index = cur_tri_index

                part = line[:-1].partition(" ")[2]
                current_part = part
                print("Loading {} ...".format(part))
            elif line.startswith("usemtl "):
                material = line[:-1].partition(" ")[2]
                print(material)
            elif line.startswith("v "):
                sl = line.split()
                vertices.append(100 * np.array((float(sl[1]), float(sl[2]), float(sl[3]))))
            elif line.startswith("f "):
                ls = line.split()
                vert_ref = [int(vert.split("/")[0]) for vert in ls[1:]]

                a = vertices[vert_ref[0] - 1]
                b = vertices[vert_ref[1] - 1]
                c = vertices[vert_ref[2] - 1]
                norm = matmath.safeNorm(np.cross(b - a, c - a))
                
                triangles.append(np.array((a, b, c, norm)))

                cur_tri_index += 1

    parts.append((start_tri_index, cur_tri_index))

    print("# of vertices: ", len(vertices))
    print("# of triangles: ", len(triangles))

    return np.array(triangles), np.array(parts)

