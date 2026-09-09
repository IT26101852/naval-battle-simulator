#include <stdio.h>
#include <stdlib.h>
#include "part2b_simulation.h"
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

void runPart2B(Battlefield *bf, const char *resultFilename) {
    Battleship *b = &bf->battleship;

    FILE *fp = fopen(resultFilename, "w");
    if (!fp) {
        printf("Error: could not open %s for writing.\n", resultFilename);
        return;
    }

    fprintf(fp, "===== PART 2-B SIMULATION RESULT (Continuous Escort Fire) =====\n");
    fprintf(fp, "Battleship reload time (T_B): %.2f s\n", b->reloadTime);
    fprintf(fp, "Escort reload times (T_E): ");
    for (int t = 0; t < NUM_ESCORT_TYPES; t++) {
        fprintf(fp, "%s=%.2f ", ESCORT_NOTATION[t], bf->escortReloadTime[t]);
    }
    fprintf(fp, "\n\n");

    Candidate candidates[MAX_ESCORT_SHIPS];
    int numCandidates = 0;

    for (int i = 0; i < bf->numEscorts; i++) {
        EscortShip *e = &bf->escorts[i];
        double range = distanceBetween(b->x, b->y, e->x, e->y);

        double vB, angB, tBtravel;
        if (!findFiringSolution(range, 0, b->maxVelocity, 0, 90, &vB, &angB, &tBtravel)) {
            continue;
        }

        Candidate c;
        c.escortIndex = i;
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

    fprintf(fp, "--- Battleship Attack Order & Escort Hit Generation ---\n");
    HitEvent hits[MAX_ESCORT_SHIPS * 20];
    int numHits = 0;

    for (int k = 0; k < numCandidates; k++) {
        Candidate *c = &candidates[k];
        EscortShip *e = &bf->escorts[c->escortIndex];

        double launchTime = k * b->reloadTime;
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
            fprintf(fp, " | fired %d successful shot(s) before being destroyed\n", shotCount);
        } else {
            fprintf(fp, " | not a threat\n");
        }

        e->isDestroyed = 1;
    }

    for (int a = 0; a < numHits - 1; a++) {
        for (int j = 0; j < numHits - 1 - a; j++) {
            if (hits[j].time > hits[j + 1].time) {
                HitEvent temp = hits[j];
                hits[j] = hits[j + 1];
                hits[j + 1] = temp;
            }
        }
    }

    fprintf(fp, "\n--- Damage applied to Battleship (in time order) ---\n");
    int sunkAtHit = -1;
    for (int h = 0; h < numHits; h++) {
        EscortShip *e = &bf->escorts[hits[h].escortIndex];
        double before = b->cumulativeDamage;
        b->cumulativeDamage += e->impactPower;
        fprintf(fp, "t=%.4f s | E%d (%s) | impact: %.2f | damage: %.2f%% -> %.2f%%\n",
                hits[h].time, e->id, ESCORT_NOTATION[e->type], e->impactPower,
                before * 100.0, b->cumulativeDamage * 100.0);
        if (b->cumulativeDamage >= 1.0 && sunkAtHit == -1) {
            sunkAtHit = h;
	                break;
        }
    }

    if (sunkAtHit != -1) {
        b->isDestroyed = 1;
        EscortShip *finalBlow = &bf->escorts[hits[sunkAtHit].escortIndex];
        fprintf(fp, "\nOUTCOME: Battleship SUNK - final blow by E%d (%s) at t=%.4f s\n",
                finalBlow->id, ESCORT_NOTATION[finalBlow->type], hits[sunkAtHit].time);
        printf("Battleship SUNK - final blow by E%d at t=%.4fs\n", finalBlow->id, hits[sunkAtHit].time);
    } else {
        fprintf(fp, "\nOUTCOME: Battleship SURVIVES with %.2f%% cumulative damage\n", b->cumulativeDamage * 100.0);
        printf("Battleship SURVIVES with %.2f%% cumulative damage\n", b->cumulativeDamage * 100.0);
    }

    fprintf(fp, "\n--- Final Battlefield Conditions ---\n");
    fprintf(fp, "Battleship position: (%.2f, %.2f) | Cumulative damage: %.2f%% | Status: %s\n",
            b->x, b->y, b->cumulativeDamage * 100.0, b->isDestroyed ? "DESTROYED" : "SURVIVED");

    int totalDestroyed = 0;
    for (int i = 0; i < bf->numEscorts; i++) {
        EscortShip *e = &bf->escorts[i];
        if (e->isDestroyed) totalDestroyed++;
        fprintf(fp, "E%d | Type: %s | Position: (%.2f, %.2f) | Status: %s\n",
                e->id, ESCORT_NOTATION[e->type], e->x, e->y,
                e->isDestroyed ? "DESTROYED" : "ALIVE");
    }
    fprintf(fp, "\nTotal escort ships destroyed: %d out of %d\n", totalDestroyed, bf->numEscorts);

    fclose(fp);
}
