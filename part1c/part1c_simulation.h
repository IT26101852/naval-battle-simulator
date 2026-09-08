#ifndef PART1C_SIMULATION_H
#define PART1C_SIMULATION_H

#include "structures.h"

typedef struct {
    double canvasSize;
    Battleship battleship;
    EscortShip escorts[MAX_ESCORT_SHIPS];
    int numEscorts;
} Battlefield;

void setupBattlefield(Battlefield *bf);

// Part 1-C, static battleship version (redo of Part 1-A with cumulative damage).
// Each E ship can only fire once. A hit adds that E's impactPower to B's
// cumulativeDamage. B is destroyed once cumulativeDamage >= 1.0.
// B still destroys any E it can hit with a single shot.
void runPart1C(Battlefield *bf, const char *resultFilename);

#endif
