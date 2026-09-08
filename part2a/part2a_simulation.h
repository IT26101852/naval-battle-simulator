#ifndef PART2A_SIMULATION_H
#define PART2A_SIMULATION_H

#include "structures.h"

typedef struct {
    double canvasSize;
    Battleship battleship;
    EscortShip escorts[MAX_ESCORT_SHIPS];
    int numEscorts;
} Battlefield;

void setupBattlefield(Battlefield *bf);

// Part 2-A: static battleship, with reload time T_B between B's shots and
// an attack-order strategy (attack the most imminent E threats first).
// Redoes Part 1-C (cumulative damage) with this timing constraint.
void runPart2A(Battlefield *bf, const char *resultFilename);

#endif
