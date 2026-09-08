#ifndef PART2A_PATH_SIMULATION_H
#define PART2A_PATH_SIMULATION_H

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

// Part 2-A applied to a moving battleship (redo of Part 1-B/1-C with reload time
// and attack-order strategy). At each path point, B's reload clock restarts
// (it has repositioned and re-aims), and it attacks candidates in order of
// urgency (most imminent threats first), same strategy as the static version.
// useJam = 0 -> Simulation 1, useJam = 1 -> Simulation 2 (gun jams after jamAfterIteration)
// Returns 1 if the battleship was sunk, 0 if it survived all k points.
int runPart2A_Path(Battlefield *bf, double *pathX, double *pathY, int k,
                    int useJam, int jamAfterIteration, double jamThetaMin,
                    const char *resultFilename);

#endif
