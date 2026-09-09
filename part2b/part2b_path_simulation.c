#include <stdio.h>
#include <stdlib.h>
#include "part2b_path_simulation.h"
#include "setup.h"
#include "physics.h"
#include "constants.h"

void setupBattlefield(Battlefield *bf) {
    bf->canvasSize = getCanvasSize();
    initializeBattleship(&bf->battleship, bf->canvasSize);
    bf->battleship.reloadTime = getReloadTime();

    bf->numEscorts = getEscortShipCount();
    if (bf->numEscorts > MAX_ESCORT_SHIPS) {
        printf("N too large, capping at %d\n", MAX_ESCORT_SHIPS);
        bf->numEscorts = MAX_ESCORT_SHIPS;
    }

    for (int i = 0; i < bf->numEscorts; i++) {
        initializeEscortShip(&bf->escorts[i], i + 1, bf->canvasSize, bf->battleship.maxVelocity);
    }

    printf("\n--- Escort reload times (T_E) per type ---\n");
    int choice;
    printf("1. Enter each manually\n2. Generate randomly\nChoice: ");
    scanf("%d", &choice);

    for (int t = 0; t < NUM_ESCORT_TYPES; t++) {
        if (choice == 1) {
            printf("Enter T_E for %s: ", ESCORT_NOTATION[t]);
            scanf("%lf", &bf->escortReloadTime[t]);
        } else {
            bf->escortReloadTime[t] = randomDouble(0.5, 4.0);
            printf("%s: T_E = %.2f\n", ESCORT_NOTATION[t], bf->escortReloadTime[t]);
        }
    }
}

void generateBattleshipPath(double *pathX, double *pathY, int k, double canvasSize) {
    for (int i = 0; i < k; i++) {
        pathX[i] = randomDouble(0, canvasSize);
        pathY[i] = randomDouble(0, canvasSize);
    }
}

typedef struct {
    int escortIndex;
    int isThreat;
    double firstAttackTime;
    double bTravelTime;
} Candidate;

typedef struct {
    int escortIndex;
    double time;
} HitEvent;

int runPart2B_Path(Battlefield *bf, double *pathX, double *pathY, int k,
                    int useJam, int jamAfterIteration, double jamThetaMin,
                    const char *resultFilename) {

    FILE *fp = fopen(resultFilename, "w");
    if (!fp) {
        printf("Error: could not open %s for writing.\n", resultFilename);
        return 0;
    }

    fprintf(fp, "===== PART 2-B SIMULATION RESULT - Moving Battleship (%s) =====\n",
            useJam ? "Simulation 2 - Jammed Gun" : "Simulation 1 - Moving Battleship");
    fprintf(fp, "Number of path points (k): %d | T_B: %.2f s\n\n", k, bf->battleship.reloadTime);

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

        fprintf(fp, "--- Iteration %d: Battleship at (%.2f, %.2f)%s [Damage so far: %.2f%%] ---\n",
                i + 1, b->x, b->y, isJammedNow ? " [GUN JAMMED]" : "", b->cumulativeDamage * 100.0);

        Candidate candidates[MAX_ESCORT_SHIPS];
        int numCandidates = 0;

        for (int j = 0; j < bf->numEscorts; j++) {
            EscortShip *e = &bf->escorts[j];
            if (e->isDestroyed) {
                continue;
            }

            double range = distanceBetween(b->x, b->y, e->x, e->y);

            double vB, angB, tBtravel;
            if (!findFiringSolution(range, 0, b->maxVelocity, thetaLowB, thetaHighB, &vB, &angB, &tBtravel)) {
                continue;
            }

            Candidate c;
            c.escortIndex = j;
            c.bTravelTime = tBtravel;
            c.isThreat = 0;
            c.firstAttackTime = 0;

            double vE, angE, tE;
            if (findFiringSolution(range, e->minVelocity, e->maxVelocity,
                                   e->minAngle, e->maxAngle, &vE, &angE, &tE)) {
                c.isThreat = 1;
                c.firstAttackTime = tE;
            }

            candidates[numCandidates++] = c;
        }

        for (int a = 0; a < numCandidates - 1; a++) {
            for (int j = 0; j < numCandidates - 1 - a; j++) {
                Candidate *x = &candidates[j];
                Candidate *y = &candidates[j + 1];
                int xBeforeY;
                if (x->isThreat && y->isThreat) {
                    xBeforeY = (x->firstAttackTime <= y->firstAttackTime);
                } else if (x->isThreat != y->isThreat) {
                    xBeforeY = x->isThreat;
                } else {
                    xBeforeY = (x->bTravelTime <= y->bTravelTime);
                }
                if (!xBeforeY) {
                    Candidate temp = *x;
                    *x = *y;
                    *y = temp;
                }
            }
        }

        HitEvent hits[MAX_ESCORT_SHIPS * 20];
        int numHits = 0;

        for (int kk = 0; kk < numCandidates; kk++) {
            Candidate *c = &candidates[kk];
            EscortShip *e = &bf->escorts[c->escortIndex];

            double launchTime = kk * b->reloadTime;
            double bImpactTime = launchTime + c->bTravelTime;

            fprintf(fp, "Target E%d (%s) | B destroys it at t=%.4f", e->id, ESCORT_NOTATION[e->type], bImpactTime);

            if (c->isThreat) {
                double tEtype = bf->escortReloadTime[e->type];
                int shotCount = 0;
                double hitTime = c->firstAttackTime;
                while (hitTime < bImpactTime && numHits < MAX_ESCORT_SHIPS * 20) {
                    hits[numHits].escortIndex = c->escortIndex;
                    hits[numHits].time = hitTime;
                    numHits++;
                    shotCount++;
                    hitTime = c->firstAttackTime + shotCount * tEtype;
                }
                fprintf(fp, " | fired %d successful shot(s)\n", shotCount);
            } else {
                fprintf(fp, " | not a threat\n");
            }

            e->isDestroyed = 1;
        }

        for (int a = 0; a < numHits - 1; a++) {
            for (int c2 = 0; c2 < numHits - 1 - a; c2++) {
                if (hits[c2].time > hits[c2 + 1].time) {
                    HitEvent temp = hits[c2];
                    hits[c2] = hits[c2 + 1];
                    hits[c2 + 1] = temp;
                }
            }
        }

        for (int h = 0; h < numHits; h++) {
            EscortShip *e = &bf->escorts[hits[h].escortIndex];
            double before = b->cumulativeDamage;
            b->cumulativeDamage += e->impactPower;
            fprintf(fp, "  -> t=%.4f s | E%d damage: %.2f%% -> %.2f%%\n",
                    hits[h].time, e->id, before * 100.0, b->cumulativeDamage * 100.0);

            if (b->cumulativeDamage >= 1.0 && !sunk) {
                sunk = 1;
                b->isDestroyed = 1;
                fprintf(fp, "OUTCOME: Battleship SUNK - final blow by E%d (%s)\n", e->id, ESCORT_NOTATION[e->type]);
                printf("Iteration %d: Battleship SUNK - final blow by E%d\n", i + 1, e->id);
                break;
            }
        }

        fprintf(fp, "\n");
        if (!sunk) {
            printf("Iteration %d: %d target(s) engaged. Battleship survives (damage %.2f%%).\n",
                   i + 1, numCandidates, b->cumulativeDamage * 100.0);
        }

        iterationsRun = i + 1;
    }

    fprintf(fp, "\n--- Final Summary ---\n");
    fprintf(fp, "Iterations completed: %d out of %d\n", iterationsRun, k);
    fprintf(fp, "Battleship cumulative damage: %.2f%%\n", b->cumulativeDamage * 100.0);
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
