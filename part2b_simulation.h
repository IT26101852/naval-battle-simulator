#ifndef PART2B_SIMULATION_H
#define PART2B_SIMULATION_H

#include "structures.h"

typedef struct {
    double canvasSize;
    Battleship battleship;
    EscortShip escorts[MAX_ESCORT_SHIPS];
    int numEscorts;
    double escortReloadTime[NUM_ESCORT_TYPES];   // T_E per escort type
} Battlefield;

void setupBattlefield(Battlefield *bf);

// Part 2-B: static battleship. B keeps its reload time + attack-order strategy
// from Part 2-A. Escort ships now fire CONTINUOUSLY (not just once) at their
// own per-type reload interval, until B destroys them or B is destroyed.
void runPart2B(Battlefield *bf, const char *resultFilename);

#endif
