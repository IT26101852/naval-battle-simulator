#ifndef PART2B_PATH_SIMULATION_H
#define PART2B_PATH_SIMULATION_H

#include "structures.h"

#define MAX_PATH_POINTS 100

typedef struct {
    double canvasSize;
    Battleship battleship;
    EscortShip escorts[MAX_ESCORT_SHIPS];
    int numEscorts;
    double escortReloadTime[NUM_ESCORT_TYPES];
} Battlefield;

void setupBattlefield(Battlefield *bf);
void generateBattleshipPath(double *pathX, double *pathY, int k, double canvasSize);

// Part 2-B applied to a moving battleship (redo of Part 1-B/1-C/2-A with
// continuous escort fire). At each path point, both B's reload clock and
// each threatening escort's firing schedule restart fresh (B has
// repositioned; local timeline per iteration, consistent with Part 2-A's
// moving version).
int runPart2B_Path(Battlefield *bf, double *pathX, double *pathY, int k,
                    int useJam, int jamAfterIteration, double jamThetaMin,
                    const char *resultFilename);

#endif
