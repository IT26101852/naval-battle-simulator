#ifndef PHYSICS_H
#define PHYSICS_H

// Returns straight-line (horizontal) distance between two points
double distanceBetween(double x1, double y1, double x2, double y2);

// Tries to find a (velocity, angle) combination within the given limits
// that makes the shell land exactly at 'range' distance.
// Returns 1 if a valid firing solution exists, 0 otherwise.
// On success, fills outVelocity, outAngle (degrees), outTime (seconds).
int findFiringSolution(double range,
                        double minVelocity, double maxVelocity,
                        double minAngleDeg, double maxAngleDeg,
                        double *outVelocity, double *outAngle, double *outTime);

#endif
