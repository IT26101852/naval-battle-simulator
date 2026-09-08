#include <stdio.h>
#include <stdlib.h>
#include "part1c_simulation.h"
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

// Holds one successful "E hits B" event, so we can sort them by time.
typedef struct {
    int escortIndex;
    double time;
} HitEvent;

void runPart1C(Battlefield *bf, const char *resultFilename) {
    Battleship *b = &bf->battleship;

    FILE *fp = fopen(resultFilename, "w");
    if (!fp) {
        printf("Error: could not open %s for writing.\n", resultFilename);
        return;
    }

    fprintf(fp, "===== PART 1-C SIMULATION RESULT (Cumulative Damage) =====\n\n");

    HitEvent hits[MAX_ESCORT_SHIPS];
    int numHits = 0;

    double bHitTime[MAX_ESCORT_SHIPS];
    int bCanHitEscort[MAX_ESCORT_SHIPS];

    // Step 1: figure out every escort that can hit B, and every escort B can hit.
    for (int i = 0; i < bf->numEscorts; i++) {
        EscortShip *e = &bf->escorts[i];
        double range = distanceBetween(b->x, b->y, e->x, e->y);

        double vB, angB, tB;
        bCanHitEscort[i] = findFiringSolution(range, 0, b->maxVelocity, 0, 90, &vB, &angB, &tB);
        if (bCanHitEscort[i]) {
            bHitTime[i] = tB;
        }

        if (!e->hasFired) {
            double vE, angE, tE;
            if (findFiringSolution(range, e->minVelocity, e->maxVelocity,
                                   e->minAngle, e->maxAngle, &vE, &angE, &tE)) {
                e->hasFired = 1;
                e->firedVelocity = vE;
                e->firedAngle = angE;
                hits[numHits].escortIndex = i;
                hits[numHits].time = tE;
                numHits++;
            }
        }
    }

    // Step 2: sort the successful E-hits by time (ascending) - simple bubble sort,
    // fine since numEscorts is small (<= 100).
    for (int i = 0; i < numHits - 1; i++) {
        for (int j = 0; j < numHits - 1 - i; j++) {
            if (hits[j].time > hits[j + 1].time) {
                HitEvent temp = hits[j];
                hits[j] = hits[j + 1];
                hits[j + 1] = temp;
            }
        }
    }

    // Step 3: apply hits to B in time order, accumulating damage.
    fprintf(fp, "--- Escort ship hits on Battleship (in time order) ---\n");
    int sunkAtHit = -1;

    for (int h = 0; h < numHits; h++) {
        EscortShip *e = &bf->escorts[hits[h].escortIndex];
        double before = b->cumulativeDamage;
        b->cumulativeDamage += e->impactPower;

        fprintf(fp, "t=%.4f s | E%d (%s) hits B | impact: %.2f | damage: %.2f%% -> %.2f%%\n",
                hits[h].time, e->id, ESCORT_NOTATION[e->type], e->impactPower,
                before * 100.0, b->cumulativeDamage * 100.0);

        if (b->cumulativeDamage >= 1.0 && sunkAtHit == -1) {
            sunkAtHit = h;
        }
    }

    if (sunkAtHit != -1) {
        b->isDestroyed = 1;
        EscortShip *finalBlow = &bf->escorts[hits[sunkAtHit].escortIndex];
        fprintf(fp, "\nOUTCOME: Battleship SUNK - final blow by E%d (%s) at t=%.4f s (cumulative damage reached %.2f%%)\n",
                finalBlow->id, ESCORT_NOTATION[finalBlow->type], hits[sunkAtHit].time,
                b->cumulativeDamage * 100.0);
        printf("Battleship SUNK - final blow by E%d at t=%.4fs (damage %.2f%%)\n",
               finalBlow->id, hits[sunkAtHit].time, b->cumulativeDamage * 100.0);
    } else {
        fprintf(fp, "\nOUTCOME: Battleship SURVIVES with %.2f%% cumulative damage\n", b->cumulativeDamage * 100.0);
        printf("Battleship SURVIVES with %.2f%% cumulative damage\n", b->cumulativeDamage * 100.0);
    }

    // Step 4: B destroys every escort it could hit (single-shot kill, unchanged from Part 1-A/B).
    fprintf(fp, "\n--- Escort ships destroyed by Battleship ---\n");
    int hitCount = 0;
    for (int i = 0; i < bf->numEscorts; i++) {
        if (bCanHitEscort[i]) {
            EscortShip *e = &bf->escorts[i];
            e->isDestroyed = 1;
            hitCount++;
            fprintf(fp, "E%d (%s) destroyed, time to hit: %.4f s\n",
                    e->id, ESCORT_NOTATION[e->type], bHitTime[i]);
        }
    }
    fprintf(fp, "Total escort ships destroyed by B: %d out of %d\n", hitCount, bf->numEscorts);
    printf("%d escort ship(s) destroyed by Battleship.\n", hitCount);

    // Step 5: final battlefield conditions.
    fprintf(fp, "\n--- Final Battlefield Conditions ---\n");
    fprintf(fp, "Battleship position: (%.2f, %.2f) | Cumulative damage: %.2f%% | Status: %s\n",
            b->x, b->y, b->cumulativeDamage * 100.0, b->isDestroyed ? "DESTROYED" : "SURVIVED");

    for (int i = 0; i < bf->numEscorts; i++) {
        EscortShip *e = &bf->escorts[i];
        fprintf(fp, "E%d | Type: %s | Position: (%.2f, %.2f) | Status: %s\n",
                e->id, ESCORT_NOTATION[e->type], e->x, e->y,
                e->isDestroyed ? "DESTROYED" : "ALIVE");
    }

    fclose(fp);
}
