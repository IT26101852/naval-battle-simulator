#ifndef PART1B_SIMULATION_H
#define PART1B_SIMULATION_H

#include "structures.h"

#define MAX_PATH_POINTS 100

typedef struct {
    double canvasSize;
    Battleship battleship;
    EscortShip escorts[MAX_ESCORT_SHIPS];
    int numEscorts;
} Battlefield;

void setupBattlefield(Battlefield *bf);
void generateBattleshipPath(double *pathX, double *pathY, int k, double canvasSize);

// useJam = 0 -> Simulation 1 (no jam, full 0-90 range every iteration)
// useJam = 1 -> Simulation 2 (after jamAfterIteration, B's angle range becomes [jamThetaMin, 90])
// Returns 1 if the battleship was sunk during the run, 0 if it survived all k points.
int runSimulationPath(Battlefield *bf, double *pathX, double *pathY, int k,
                       int useJam, int jamAfterIteration, double jamThetaMin,
                       const char *resultFilename);

#endif
