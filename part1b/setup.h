#ifndef SETUP_H
#define SETUP_H

#include "structures.h"

// Random number helpers
double randomDouble(double min, double max);
int randomInt(int min, int max);

// Battlefield setup functions
double getCanvasSize(void);              // D value (user input or random)
int getEscortShipCount(void);            // N value (user input)
BattleshipType getBattleshipType(void);  // U/M/R/S from user

void initializeBattleship(Battleship *b, double canvasSize);
void initializeEscortShip(EscortShip *e, int id, double canvasSize, double vMaxB);

#endif
