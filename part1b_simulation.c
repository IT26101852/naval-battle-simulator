#include <stdio.h>
#include <stdlib.h>
#include "part1b_simulation.h"
#include "setup.h"
#include "physics.h"
#include "constants.h"

void setupBattlefield(Battlefield *bf) {
    bf->canvasSize = getCanvasSize();
    initializeBattleship(&bf->battleship, bf->canvasSize);

    bf->numEscorts = getEscortShipCount();
    if (bf->numEscorts > MAX_ESCORT_SHIPS) {
        printf("N too large, capping at %d\n", MAX_ESCORT_SHIPS);
        bf->numEscorts = MAX_ESCORT_SHIPS;
    }

    for (int i = 0; i < bf->numEscorts; i++) {
        initializeEscortShip(&bf->escorts[i], i + 1, bf->canvasSize, bf->battleship.maxVelocity);
    }
}

void generateBattleshipPath(double *pathX, double *pathY, int k, double canvasSize) {
    for (int i = 0; i < k; i++) {
        pathX[i] = randomDouble(0, canvasSize);
        pathY[i] = randomDouble(0, canvasSize);
    }
}

int runSimulationPath(Battlefield *bf, double *pathX, double *pathY, int k,
                       int useJam, int jamAfterIteration, double jamThetaMin,
                       const char *resultFilename) {

    FILE *fp = fopen(resultFilename, "w");
    if (!fp) {
        printf("Error: could not open %s for writing.\n", resultFilename);
        return 0;
    }

    fprintf(fp, "===== PART 1-B SIMULATION RESULT (%s) =====\n",
            useJam ? "Simulation 2 - Jammed Gun" : "Simulation 1 - Moving Battleship");
    fprintf(fp, "Number of path points (k): %d\n\n", k);

    Battleship *b = &bf->battleship;
    int sunk = 0;
    int iterationsRun = 0;

    for (int i = 0; i < k && !sunk; i++) {
        b->x = pathX[i];
        b->y = pathY[i];

        double thetaLowB = 0.0, thetaHighB = 90.0;
        int isJammedNow = (useJam && i >= jamAfterIteration);
        if (isJammedNow) {
            thetaLowB = jamThetaMin;
        }

        fprintf(fp, "--- Iteration %d: Battleship at (%.2f, %.2f)%s ---\n",
                i + 1, b->x, b->y, isJammedNow ? " [GUN JAMMED]" : "");

        int sinkingIdx = -1;
        double bestTime = 1e18;
        double bHitTime[MAX_ESCORT_SHIPS];
        int bCanHit[MAX_ESCORT_SHIPS];

        for (int j = 0; j < bf->numEscorts; j++) {
            EscortShip *e = &bf->escorts[j];
            bCanHit[j] = 0;

            if (e->isDestroyed) {
                continue;
            }

            double range = distanceBetween(b->x, b->y, e->x, e->y);

            double vB, angB, tB;
            bCanHit[j] = findFiringSolution(range, 0, b->maxVelocity, thetaLowB, thetaHighB, &vB, &angB, &tB);
            if (bCanHit[j]) {
                bHitTime[j] = tB;
            }

            if (!e->hasFired) {
                double vE, angE, tE;
                if (findFiringSolution(range, e->minVelocity, e->maxVelocity,
                                       e->minAngle, e->maxAngle, &vE, &angE, &tE)) {
                    e->hasFired = 1;
                    if (tE < bestTime) {
                        bestTime = tE;
                        sinkingIdx = j;
                        e->firedVelocity = vE;
                        e->firedAngle = angE;
                    }
                }
            }
        }

        if (sinkingIdx != -1) {
            b->isDestroyed = 1;
            sunk = 1;
            EscortShip *sinker = &bf->escorts[sinkingIdx];
            fprintf(fp, "OUTCOME: Battleship SUNK by E%d (%s) at t=%.4f s\n",
                    sinker->id, ESCORT_NOTATION[sinker->type], bestTime);
            iterationsRun = i + 1;
            printf("Iteration %d: Battleship SUNK by E%d at t=%.4fs\n", i + 1, sinker->id, bestTime);
            break;
        }

        int hitCount = 0;
        for (int j = 0; j < bf->numEscorts; j++) {
            if (bCanHit[j]) {
                EscortShip *e = &bf->escorts[j];
                e->isDestroyed = 1;
                hitCount++;
                fprintf(fp, "E%d (%s) destroyed, time to hit: %.4f s\n",
                        e->id, ESCORT_NOTATION[e->type], bHitTime[j]);
            }
        }
        fprintf(fp, "Escort ships destroyed this iteration: %d\n\n", hitCount);
        printf("Iteration %d: %d escort ship(s) destroyed. Battleship survives.\n", i + 1, hitCount);

        iterationsRun = i + 1;
    }

    fprintf(fp, "\n--- Final Summary ---\n");
    fprintf(fp, "Iterations completed: %d out of %d\n", iterationsRun, k);
    fprintf(fp, "Battleship status: %s\n", sunk ? "DESTROYED" : "SURVIVED");

    fprintf(fp, "\n--- Final Escort Ship Status ---\n");
    int totalDestroyed = 0;
    for (int j = 0; j < bf->numEscorts; j++) {
        EscortShip *e = &bf->escorts[j];
        if (e->isDestroyed) {
            totalDestroyed++;
        }
        fprintf(fp, "E%d | Type: %s | Position: (%.2f, %.2f) | Status: %s\n",
                e->id, ESCORT_NOTATION[e->type], e->x, e->y,
                e->isDestroyed ? "DESTROYED" : "ALIVE");
    }
    fprintf(fp, "\nTotal escort ships destroyed: %d out of %d\n", totalDestroyed, bf->numEscorts);

    fclose(fp);
    return sunk;
}
