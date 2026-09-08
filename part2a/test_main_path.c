#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "setup.h"
#include "physics.h"
#include "part2a_path_simulation.h"

int main(void) {
    srand(time(NULL));

    Battlefield bf;
    setupBattlefield(&bf);

    int k;
    printf("Enter number of path points (k): ");
    scanf("%d", &k);
    if (k > MAX_PATH_POINTS) k = MAX_PATH_POINTS;

    double pathX[MAX_PATH_POINTS], pathY[MAX_PATH_POINTS];
    generateBattleshipPath(pathX, pathY, k, bf.canvasSize);

    printf("\n--- Simulation 1 (no jam) ---\n");
    runPart2A_Path(&bf, pathX, pathY, k, 0, 0, 0, "part2a_sim1_result.txt");

    for (int i = 0; i < bf.numEscorts; i++) {
        bf.escorts[i].isDestroyed = 0;
        bf.escorts[i].hasFired = 0;
    }
    bf.battleship.isDestroyed = 0;
    bf.battleship.cumulativeDamage = 0.0;

    int t;
    double thetaMin;
    printf("\nEnter iteration number after which the gun jams (t): ");
    scanf("%d", &t);
    printf("Enter theta_min for jammed gun (0-30): ");
    scanf("%lf", &thetaMin);

    printf("\n--- Simulation 2 (jammed gun) ---\n");
    runPart2A_Path(&bf, pathX, pathY, k, 1, t, thetaMin, "part2a_sim2_result.txt");

    return 0;
}
