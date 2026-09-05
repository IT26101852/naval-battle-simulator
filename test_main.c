#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "setup.h"
#include "physics.h"
#include "simulation.h"

int main(void) {
    srand(42);

    Battlefield bf;
    setupBattlefield(&bf);
    saveInitialConditions(&bf, "initial_conditions.txt");
    runPart1A(&bf, "part1a_result.txt");

    return 0;
}
