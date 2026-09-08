#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "setup.h"
#include "physics.h"
#include "part2a_simulation.h"

int main(void) {
    srand(time(NULL));

    Battlefield bf;
    setupBattlefield(&bf);
    runPart2A(&bf, "part2a_result.txt");

    return 0;
}
