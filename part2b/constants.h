#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "structures.h"

// Gravity (m/s^2)
#define GRAVITY 9.81

// ---- Table 1: Escort ship type properties ----
// Index order matches EscortType enum: A, B, C, D, E

static const double ESCORT_IMPACT_POWER[NUM_ESCORT_TYPES] = {
    0.08,   // E_A - 1936A-class Destroyer
    0.06,   // E_B - Gabbiano-class Corvette
    0.07,   // E_C - Matsu-class Destroyer
    0.05,   // E_D - F-class Escort Ships
    0.04    // E_E - Japanese Kaibokan
};

static const int ESCORT_ANGLE_RANGE[NUM_ESCORT_TYPES] = {
    20,   // E_A
    30,   // E_B
    25,   // E_C
    50,   // E_D
    70    // E_E
};

static const char *ESCORT_TYPE_NAME[NUM_ESCORT_TYPES] = {
    "1936A-class Destroyer",
    "Gabbiano-class Corvette",
    "Matsu-class Destroyer",
    "F-class Escort Ships",
    "Japanese Kaibokan"
};

static const char *ESCORT_NOTATION[NUM_ESCORT_TYPES] = {
    "EA", "EB", "EC", "ED", "EE"
};

// ---- Battleship type names ----
static const char *BATTLESHIP_TYPE_NAME[4] = {
    "USS Iowa (BB-61)",
    "MS King George V",
    "Richelieu",
    "Sovetsky Soyuz-class"
};

static const char *BATTLESHIP_NOTATION[4] = {
    "U", "M", "R", "S"
};

#endif
