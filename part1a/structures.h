#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_ESCORT_SHIPS 100

typedef enum {
    E_TYPE_A,   // 1936A-class Destroyer
    E_TYPE_B,   // Gabbiano-class Corvette
    E_TYPE_C,   // Matsu-class Destroyer
    E_TYPE_D,   // F-class Escort Ships
    E_TYPE_E,   // Japanese Kaibokan
    NUM_ESCORT_TYPES
} EscortType;

typedef enum {
    B_TYPE_U,   // USS Iowa (BB-61)
    B_TYPE_M,   // MS King George V
    B_TYPE_R,   // Richelieu
    B_TYPE_S    // Sovetsky Soyuz-class
} BattleshipType;

typedef struct {
    int id;
    EscortType type;
    double x, y;

    double impactPower;   // fixed value from Table 1 (e.g. 0.08 for E_A)
    int angleRange;        // fixed value from Table 1 (thetaH - thetaL)

    double minAngle;       // theta_L (randomly generated)
    double maxAngle;        // theta_H = minAngle + angleRange
    double minVelocity;     // randomly generated
    double maxVelocity;     // randomly generated

    double firedVelocity;   // actual speed used when this ship fires
    double firedAngle;      // actual vertical angle used when firing

    int isDestroyed;        // 1 = destroyed, 0 = alive
    int hasFired;           // 1 = already fired once (Part 1-A rule)
} EscortShip;

typedef struct {
    BattleshipType type;
    double x, y;

    double maxVelocity;     // Vmax_B (user input or random)

    double firedVelocity;   // actual speed used for a shot
    double firedAngle;      // actual angle (0-90) used for a shot

    int isDestroyed;
} Battleship;

#endif
