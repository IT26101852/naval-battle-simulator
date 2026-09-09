#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "setup.h"
#include "physics.h"
#include "part2b_simulation.h"

int main(void) {
    srand(time(NULL));

    Battlefield bf;
    setupBattlefield(&bf);
    runPart2B(&bf, "part2b_result.txt");

    return 0;
}
