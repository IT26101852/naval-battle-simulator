#ifndef PART1C_PATH_SIMULATION_H
#define PART1C_PATH_SIMULATION_H

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

// useJam = 0 -> Simulation 1 (no jam)
// useJam = 1 -> Simulation 2 (gun jams after jamAfterIteration)
// Returns 1 if the battleship was sunk (cumulative damage reached 100%), 0 if it survived all k points.
int runPart1C_Path(Battlefield *bf, double *pathX, double *pathY, int k,
                    int useJam, int jamAfterIteration, double jamThetaMin,
                    const char *resultFilename);

#endif
