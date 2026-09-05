#ifndef SIMULATION_H
#define SIMULATION_H

#include "structures.h"

typedef struct {
    double canvasSize;
    Battleship battleship;
    EscortShip escorts[MAX_ESCORT_SHIPS];
    int numEscorts;
} Battlefield;

void setupBattlefield(Battlefield *bf);
void saveInitialConditions(const Battlefield *bf, const char *filename);
int runPart1A(Battlefield *bf, const char *resultFilename);

#endif
