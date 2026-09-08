#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "constants.h"
#include "setup.h"
#include "physics.h"
#include "part1c_simulation.h"

int main(void) {
    srand(time(NULL));

    Battlefield bf;
    setupBattlefield(&bf);
    runPart1C(&bf, "part1c_result.txt");

    return 0;
}
