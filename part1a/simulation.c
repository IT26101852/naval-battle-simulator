#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulation.h"
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

void saveInitialConditions(const Battlefield *bf, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not open %s for writing.\n", filename);
        return;
    }

    fprintf(fp, "===== INITIAL BATTLEFIELD CONDITIONS =====\n");
    fprintf(fp, "Canvas size (D): %.2f (battlefield is %.2f x %.2f)\n\n", bf->canvasSize, bf->canvasSize, bf->canvasSize);

    fprintf(fp, "--- Battleship ---\n");
    fprintf(fp, "Type: %s (%s)\n", BATTLESHIP_TYPE_NAME[bf->battleship.type], BATTLESHIP_NOTATION[bf->battleship.type]);
    fprintf(fp, "Position: (%.2f, %.2f)\n", bf->battleship.x, bf->battleship.y);
    fprintf(fp, "Max shell velocity (Vmax_B): %.2f\n\n", bf->battleship.maxVelocity);

    fprintf(fp, "--- Escort Ships (N = %d) ---\n", bf->numEscorts);
    for (int i = 0; i < bf->numEscorts; i++) {
        const EscortShip *e = &bf->escorts[i];
        fprintf(fp, "E%d | Type: %s (%s) | Position: (%.2f, %.2f) | ImpactPower: %.2f | "
                     "AngleRange: %d | MinAngle: %.2f | MaxAngle: %.2f | MinV: %.2f | MaxV: %.2f\n",
                e->id, ESCORT_TYPE_NAME[e->type], ESCORT_NOTATION[e->type],
                e->x, e->y, e->impactPower, e->angleRange,
                e->minAngle, e->maxAngle, e->minVelocity, e->maxVelocity);
    }

    fclose(fp);
    printf("Initial conditions saved to %s\n", filename);
}

int runPart1A(Battlefield *bf, const char *resultFilename) {
    Battleship *b = &bf->battleship;

    int sinkingEscortIndex = -1;   // index into bf->escorts[], -1 = B survives
    double bestTimeToSinkB = 1e18;

    double bHitTime[MAX_ESCORT_SHIPS];   // time for B's shell to hit escort i (if possible)
    int bCanHitEscort[MAX_ESCORT_SHIPS];

    // Step 1: for every escort ship, check both directions of fire
    for (int i = 0; i < bf->numEscorts; i++) {
        EscortShip *e = &bf->escorts[i];
        double range = distanceBetween(b->x, b->y, e->x, e->y);

        // Can B hit this E?
        double vB, angleB, timeB;
        bCanHitEscort[i] = findFiringSolution(range, 0, b->maxVelocity, 0, 90, &vB, &angleB, &timeB);
        if (bCanHitEscort[i]) {
            bHitTime[i] = timeB;
        }

        // Can this E hit B? (only if it hasn't fired yet - Part 1-A: each E fires once)
        if (!e->hasFired) {
            double vE, angleE, timeE;
            int eCanHitB = findFiringSolution(range, e->minVelocity, e->maxVelocity,
                                               e->minAngle, e->maxAngle, &vE, &angleE, &timeE);
            if (eCanHitB) {
                e->hasFired = 1;
                if (timeE < bestTimeToSinkB) {
                    bestTimeToSinkB = timeE;
                    sinkingEscortIndex = i;
                    bf->escorts[i].firedVelocity = vE;
                    bf->escorts[i].firedAngle = angleE;
                }
            }
        }
    }

    FILE *fp = fopen(resultFilename, "w");
    if (!fp) {
        printf("Error: could not open %s for writing.\n", resultFilename);
        return 0;
    }

    if (sinkingEscortIndex != -1) {
        // Battleship sinks
        b->isDestroyed = 1;
        EscortShip *sinker = &bf->escorts[sinkingEscortIndex];

        fprintf(fp, "===== PART 1-A SIMULATION RESULT =====\n");
        fprintf(fp, "OUTCOME: Battleship SUNK\n");
        fprintf(fp, "Sunk by Escort Ship index: %d (Type: %s)\n", sinker->id, ESCORT_NOTATION[sinker->type]);
        fprintf(fp, "Time to sink: %.4f seconds\n", bestTimeToSinkB);
        fprintf(fp, "Fired velocity: %.2f, angle: %.2f\n\n", sinker->firedVelocity, sinker->firedAngle);

        fprintf(fp, "--- Final Battlefield Conditions ---\n");
        fprintf(fp, "Battleship position: (%.2f, %.2f) - DESTROYED\n", b->x, b->y);
        for (int i = 0; i < bf->numEscorts; i++) {
            EscortShip *e = &bf->escorts[i];
            fprintf(fp, "E%d | Type: %s | Position: (%.2f, %.2f) | Status: %s\n",
                    e->id, ESCORT_NOTATION[e->type], e->x, e->y,
                    e->isDestroyed ? "DESTROYED" : "ALIVE");
        }

        printf("Battleship SUNK by E%d (%s) at t=%.4fs\n", sinker->id, ESCORT_NOTATION[sinker->type], bestTimeToSinkB);
        fclose(fp);
        return 1;
    }

    // Battleship survives: B destroys every escort ship it can hit
    int hitCount = 0;
    double battleDuration = 0.0;

    fprintf(fp, "===== PART 1-A SIMULATION RESULT =====\n");
    fprintf(fp, "OUTCOME: Battleship SURVIVES\n\n");
    fprintf(fp, "--- Escort ships hit by Battleship ---\n");

    for (int i = 0; i < bf->numEscorts; i++) {
        if (bCanHitEscort[i]) {
            EscortShip *e = &bf->escorts[i];
            e->isDestroyed = 1;
            hitCount++;
            if (bHitTime[i] > battleDuration) {
                battleDuration = bHitTime[i];
            }
            fprintf(fp, "E%d | Type: %s | Time to hit: %.4f seconds\n",
                    e->id, ESCORT_NOTATION[e->type], bHitTime[i]);
        }
    }

    fprintf(fp, "\nTotal escort ships hit: %d out of %d\n", hitCount, bf->numEscorts);
    fprintf(fp, "Battle duration: %.4f seconds\n\n", battleDuration);

    fprintf(fp, "--- Final Battlefield Conditions ---\n");
    fprintf(fp, "Battleship position: (%.2f, %.2f) - SURVIVED\n", b->x, b->y);
    for (int i = 0; i < bf->numEscorts; i++) {
        EscortShip *e = &bf->escorts[i];
        fprintf(fp, "E%d | Type: %s | Position: (%.2f, %.2f) | Status: %s\n",
                e->id, ESCORT_NOTATION[e->type], e->x, e->y,
                e->isDestroyed ? "DESTROYED" : "ALIVE");
    }

    printf("Battleship SURVIVES. %d/%d escort ships destroyed. Battle duration: %.4fs\n",
           hitCount, bf->numEscorts, battleDuration);

    fclose(fp);
    return 0;
}
