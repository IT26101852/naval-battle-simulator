#ifndef SETUP_H
#define SETUP_H

#include "structures.h"

double randomDouble(double min, double max);
int randomInt(int min, int max);

double getCanvasSize(void);
double getReloadTime(void);
int getEscortShipCount(void);
BattleshipType getBattleshipType(void);

void initializeBattleship(Battleship *b, double canvasSize);
void initializeEscortShip(EscortShip *e, int id, double canvasSize, double vMaxB);

#endif
