#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_ESCORT_SHIPS 100
#define MAX_PATH_POINTS 100

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

typedef struct {
    double canvasSize;
    Battleship battleship;
    EscortShip escorts[MAX_ESCORT_SHIPS];
    int numEscorts;
    double escortReloadTime[NUM_ESCORT_TYPES];
} Battlefield;

#endif
