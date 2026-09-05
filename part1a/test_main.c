#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "setup.h"
#include "physics.h"

int main(void) {
    srand(42);

    Battleship b = {0};
    b.type = B_TYPE_U;
    b.x = 100; b.y = 100;
    b.maxVelocity = 500;
    b.isDestroyed = 0;

    EscortShip e;
    initializeEscortShip(&e, 1, 1000.0, b.maxVelocity);

    printf("Escort ship %d created: type=%s x=%.2f y=%.2f impactPower=%.2f minV=%.2f maxV=%.2f minAngle=%.2f maxAngle=%.2f\n",
           e.id, ESCORT_NOTATION[e.type], e.x, e.y, e.impactPower, e.minVelocity, e.maxVelocity, e.minAngle, e.maxAngle);

    double range = distanceBetween(b.x, b.y, e.x, e.y);
    printf("Distance between B and E1: %.2f\n", range);

    double v, angle, t;
    int found = findFiringSolution(range, 0, b.maxVelocity, 0, 90, &v, &angle, &t);
    if (found) {
        printf("B can hit E1: velocity=%.2f angle=%.2f time=%.2f\n", v, angle, t);
    } else {
        printf("B cannot hit E1 with given constraints.\n");
    }

    return 0;
}
