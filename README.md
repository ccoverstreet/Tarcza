# Tarcza

Tarcza is my side project for utilizing OBJ file formats from standard CAD software as the geometry inputs for shielding calculator. This program is designed to estimate gamma attenuation by using a series of rays that probe the geometry. The benefit of using OBJ files is that standard CAD software can be used to generate the detector geometries.

Currently, the parser only supports OnShape-style OBJ file outputs. It seems like every software uses a different style when outputing these OBJ files, so the parser needs a few upgrades to handle these differences. Really, the only information needed from the files is the material/part that each vertex is a part of and the triangles needed to make the geometry. The normals are easy to compute on file load.

This program could also be extended to generate MCNP style input files based upon user specifications

## Tasks

3. Validate results with comparisons to prior work and simple analytically solvable examples
	- Maybe a cylindrical detector of Ge at a set distance from the source.
		- Calculate expected counts within the detector
		- Would only look at full energy deposition
