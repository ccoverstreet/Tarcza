// Tarcza: Tracing Routine
// Cale Overstreet
// Oct. 15, 2021
/* This routine is the primary calculation loop for Tarcza. 
 */

#pragma once

#include "./objparser.h"
#include "./types.h"

void tarczaTracingRoutine(Geometry geom, Ray ray);
void traceRayPath(Geometry geom, Ray ray);
