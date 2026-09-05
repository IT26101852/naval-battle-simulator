#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include "constants.h"

double randomDouble(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

double getCanvasSize(void) {
    int choice;
    double D;

    printf("Canvas size (D) - lower-left (0,0), upper-right (D,D)\n");
    printf("1. Enter D manually\n2. Generate randomly\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter D: ");
        scanf("%lf", &D);
    } else {
        D = randomDouble(500.0, 2000.0);   // reasonable battlefield size in meters
        printf("Randomly generated D = %.2f\n", D);
    }
    return D;
}

int getEscortShipCount(void) {
    int N;
    printf("Enter number of escort ships (N): ");
    scanf("%d", &N);
    return N;
}

BattleshipType getBattleshipType(void) {
    char input[5];
    printf("Select Battleship type:\n");
    printf("U - USS Iowa (BB-61)\nM - MS King George V\nR - Richelieu\nS - Sovetsky Soyuz-class\n");
    printf("Enter notation (U/M/R/S): ");
    scanf("%s", input);

    switch (input[0]) {
        case 'U': case 'u': return B_TYPE_U;
        case 'M': case 'm': return B_TYPE_M;
        case 'R': case 'r': return B_TYPE_R;
        case 'S': case 's': return B_TYPE_S;
        default:
            printf("Invalid input, defaulting to U.\n");
            return B_TYPE_U;
    }
}

void initializeBattleship(Battleship *b, double canvasSize) {
    int choice;

    b->type = getBattleshipType();

    printf("Battleship starting position:\n1. Enter manually\n2. Generate randomly\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter x y: ");
        scanf("%lf %lf", &b->x, &b->y);
    } else {
        b->x = randomDouble(0, canvasSize);
        b->y = randomDouble(0, canvasSize);
        printf("Randomly generated position: (%.2f, %.2f)\n", b->x, b->y);
    }

    printf("Maximum shell velocity (Vmax_B):\n1. Enter manually\n2. Generate randomly\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Vmax_B: ");
        scanf("%lf", &b->maxVelocity);
    } else {
        b->maxVelocity = randomDouble(200.0, 800.0);   // m/s, reasonable range
        printf("Randomly generated Vmax_B = %.2f\n", b->maxVelocity);
    }

    b->isDestroyed = 0;
}

void initializeEscortShip(EscortShip *e, int id, double canvasSize, double vMaxB) {
    e->id = id;
    e->type = randomInt(0, NUM_ESCORT_TYPES - 1);

    e->x = randomDouble(0, canvasSize);
    e->y = randomDouble(0, canvasSize);

    e->impactPower = ESCORT_IMPACT_POWER[e->type];
    e->angleRange = ESCORT_ANGLE_RANGE[e->type];

    e->minAngle = randomDouble(0, 90 - e->angleRange);
    e->maxAngle = e->minAngle + e->angleRange;

    e->minVelocity = randomDouble(50.0, 200.0);

    if (e->type == E_TYPE_A) {
        e->maxVelocity = 1.2 * vMaxB;   // special rule for E_A
    } else {
        // must be less than Vmax_B
        e->maxVelocity = randomDouble(e->minVelocity, vMaxB);
    }

    e->isDestroyed = 0;
    e->hasFired = 0;
}
