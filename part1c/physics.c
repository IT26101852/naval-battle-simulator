#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <math.h>
#include "physics.h"
#include "constants.h"

double distanceBetween(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

int findFiringSolution(double range,
                        double minVelocity, double maxVelocity,
                        double minAngleDeg, double maxAngleDeg,
                        double *outVelocity, double *outAngle, double *outTime) {

    double step = 0.5;   // degree increments while searching

    for (double thetaDeg = minAngleDeg; thetaDeg <= maxAngleDeg; thetaDeg += step) {

        double thetaRad = thetaDeg * M_PI / 180.0;
        double sin2Theta = sin(2 * thetaRad);

        if (sin2Theta <= 0.0001) {
            continue;   // avoid division by ~0 near 0 or 90 degrees
        }

        // R = u^2 * sin(2*theta) / g  =>  u = sqrt(R * g / sin(2*theta))
        double requiredU = sqrt((range * GRAVITY) / sin2Theta);

        if (requiredU >= minVelocity && requiredU <= maxVelocity) {
            double thetaRadForTime = thetaRad;
            double timeToHit = (2 * requiredU * sin(thetaRadForTime)) / GRAVITY;

            *outVelocity = requiredU;
            *outAngle = thetaDeg;
            *outTime = timeToHit;
            return 1;   // solution found
        }
    }

    return 0;   // no valid (velocity, angle) combination found
}
