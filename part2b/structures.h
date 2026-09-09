#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_ESCORT_SHIPS 100

typedef enum {
    E_TYPE_A,
    E_TYPE_B,
    E_TYPE_C,
    E_TYPE_D,
    E_TYPE_E,
    NUM_ESCORT_TYPES
} EscortType;

typedef enum {
    B_TYPE_U,
    B_TYPE_M,
    B_TYPE_R,
    B_TYPE_S
} BattleshipType;

typedef struct {
    int id;
    EscortType type;
    double x, y;

    double impactPower;
    int angleRange;

    double minAngle;
    double maxAngle;
    double minVelocity;
    double maxVelocity;

    double firedVelocity;
    double firedAngle;

    int isDestroyed;
    int hasFired;
} EscortShip;

typedef struct {
    BattleshipType type;
    double x, y;

    double maxVelocity;

    double firedVelocity;
    double firedAngle;

    int isDestroyed;

    double cumulativeDamage;
    double reloadTime;
} Battleship;

#endif
