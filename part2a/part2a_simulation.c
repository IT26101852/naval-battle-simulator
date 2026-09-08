#include <stdio.h>
#include <stdlib.h>
#include "part2a_simulation.h"
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
}

// One candidate target in B's attack order.
typedef struct {
    int escortIndex;
    int isThreat;        // 1 if this E can hit B
    double eAttackTime;  // time E's shell would land on B (only valid if isThreat)
    double bTravelTime;  // time B's shell takes to reach this E
} Candidate;

// One successful "E hits B" event (shell landed before B destroyed that E).
typedef struct {
    int escortIndex;
    double time;
} HitEvent;

void runPart2A(Battlefield *bf, const char *resultFilename) {
    Battleship *b = &bf->battleship;

    FILE *fp = fopen(resultFilename, "w");
    if (!fp) {
        printf("Error: could not open %s for writing.\n", resultFilename);
        return;
    }

    fprintf(fp, "===== PART 2-A SIMULATION RESULT (Reload Time + Attack Order) =====\n");
    fprintf(fp, "Battleship reload time (T_B): %.2f s\n\n", b->reloadTime);

    Candidate candidates[MAX_ESCORT_SHIPS];
    int numCandidates = 0;

    // Step 1: figure out, for every escort, whether B can hit it and whether it can hit B.
    for (int i = 0; i < bf->numEscorts; i++) {
        EscortShip *e = &bf->escorts[i];
        double range = distanceBetween(b->x, b->y, e->x, e->y);

        double vB, angB, tBtravel;
        int bCanHit = findFiringSolution(range, 0, b->maxVelocity, 0, 90, &vB, &angB, &tBtravel);

        if (!bCanHit) {
            continue;   // B cannot reach this E at all - not a candidate
        }

        Candidate c;
        c.escortIndex = i;
        c.bTravelTime = tBtravel;
        c.isThreat = 0;
        c.eAttackTime = 0;

        if (!e->hasFired) {
            double vE, angE, tE;
            if (findFiringSolution(range, e->minVelocity, e->maxVelocity,
                                   e->minAngle, e->maxAngle, &vE, &angE, &tE)) {
                e->hasFired = 1;
                c.isThreat = 1;
                c.eAttackTime = tE;
            }
        }

        candidates[numCandidates++] = c;
    }

    // Step 2: sort candidates - threats first (ascending eAttackTime = most imminent first),
    // then non-threats (order doesn't matter for strategy, sort by bTravelTime for neatness).
    // Simple bubble sort with a custom "comes before" rule.
    for (int i = 0; i < numCandidates - 1; i++) {
        for (int j = 0; j < numCandidates - 1 - i; j++) {
            Candidate *a = &candidates[j];
            Candidate *c = &candidates[j + 1];
            int aBeforeC;
            if (a->isThreat && c->isThreat) {
                aBeforeC = (a->eAttackTime <= c->eAttackTime);
            } else if (a->isThreat != c->isThreat) {
                aBeforeC = a->isThreat;   // threats always come before non-threats
            } else {
                aBeforeC = (a->bTravelTime <= c->bTravelTime);
            }
            if (!aBeforeC) {
                Candidate temp = *a;
                *a = *c;
                *c = temp;
            }
        }
    }

    // Step 3: "fire" at candidates in this order, respecting the reload time gap.
    fprintf(fp, "--- Battleship Attack Order ---\n");
    HitEvent hits[MAX_ESCORT_SHIPS];
    int numHits = 0;

    for (int k = 0; k < numCandidates; k++) {
        Candidate *c = &candidates[k];
        EscortShip *e = &bf->escorts[c->escortIndex];

        double launchTime = k * b->reloadTime;
        double impactTimeOnTarget = launchTime + c->bTravelTime;

        if (c->isThreat) {
            if (impactTimeOnTarget < c->eAttackTime) {
                fprintf(fp, "Target E%d (%s) | B launches at t=%.4f, impacts at t=%.4f | "
                             "PREEMPTED before its shell (would land t=%.4f)\n",
                        e->id, ESCORT_NOTATION[e->type], launchTime, impactTimeOnTarget, c->eAttackTime);
            } else {
                fprintf(fp, "Target E%d (%s) | B launches at t=%.4f, impacts at t=%.4f | "
                             "shell already landed at t=%.4f - B takes damage\n",
                        e->id, ESCORT_NOTATION[e->type], launchTime, impactTimeOnTarget, c->eAttackTime);
                hits[numHits].escortIndex = c->escortIndex;
                hits[numHits].time = c->eAttackTime;
                numHits++;
            }
        } else {
            fprintf(fp, "Target E%d (%s) | B launches at t=%.4f, impacts at t=%.4f | not a threat\n",
                    e->id, ESCORT_NOTATION[e->type], launchTime, impactTimeOnTarget);
        }

        e->isDestroyed = 1;
    }

    // Step 4: sort successful hit events by time and apply cumulative damage.
    for (int i = 0; i < numHits - 1; i++) {
        for (int j = 0; j < numHits - 1 - i; j++) {
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

    // Step 5: final conditions.
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
