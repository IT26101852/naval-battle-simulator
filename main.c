#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void showMainMenu(void);
void showInstructions(void);
void setupMenu(void);
void startSimulation(void);
void showStatistics(void);

unsigned int simulationSeed = 0;

int main(void)
{
    int choice;

    simulationSeed = (unsigned int)time(NULL);
    srand(simulationSeed);

    do
    {
        showMainMenu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                startSimulation();
                break;

            case 2:
                showInstructions();
                break;

            case 3:
                showStatistics();
                break;

            case 4:
                printf("\nExiting Naval Battle Simulator...\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 4);

    return 0;
}


/* =========================
   MAIN MENU
   ========================= */

void showMainMenu(void)
{
    printf("\n========================================\n");
    printf("       ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("========================================\n");
    printf("1. Start Simulation\n");
    printf("2. View Instructions\n");
    printf("3. Simulation Statistics\n");
    printf("4. Exit\n");
    printf("========================================\n");
}


/* =========================
   INSTRUCTIONS
   ========================= */

void showInstructions(void)
{
    printf("\n========== INSTRUCTIONS ==========\n");
    printf("1. Select Start Simulation from the main menu.\n");
    printf("2. Select the required simulation part.\n");
    printf("3. Use Setup Menu to configure the random seed.\n");
    printf("4. View saved output files under Simulation Statistics.\n");
    printf("==================================\n");
}


/* =========================
   SETUP MENU
   ========================= */

void setupMenu(void)
{
    int choice;
    unsigned int newSeed;

    do
    {
        printf("\n========== SETUP MENU ==========\n");
        printf("1. Battleship Properties\n");
        printf("2. Escort Settings\n");
        printf("3. Seed Value\n");
        printf("4. Return to Main Menu\n");
        printf("================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("\nBattleship properties are handled by the simulation.\n");
                break;

            case 2:
                printf("\nEscort settings are handled by the simulation.\n");
                break;

            case 3:
                printf("\nCurrent seed: %u\n", simulationSeed);
                printf("Enter new seed value: ");
                scanf("%u", &newSeed);

                simulationSeed = newSeed;
                srand(simulationSeed);

                printf("Seed changed to %u successfully.\n",
                       simulationSeed);
                break;

            case 4:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                printf("\nInvalid choice.\n");
        }

    } while (choice != 4);
}


/* =========================
   START SIMULATION
   ========================= */

void startSimulation(void)
{
    int choice;

    printf("\n---------- START SIMULATION ----------\n");
    printf("1. Part 1-A\n");
    printf("2. Part 1-B\n");
    printf("3. Part 1-C\n");
    printf("4. Part 2-A\n");
    printf("5. Part 2-B\n");
    printf("6. Setup Menu\n");
    printf("7. Return to Main Menu\n");

    printf("Select simulation: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            printf("\nRunning Part 1-A...\n");
            system("./part1a/test_program");
            break;

        case 2:
            printf("\nRunning Part 1-B...\n");
            system("./part1b/test_program");
            break;

        case 3:
            printf("\nRunning Part 1-C...\n");
            system("./part1c/test_program");
            break;

        case 4:
            printf("\nRunning Part 2-A...\n");
            system("./part2a/test_program");
            break;

        case 5:
            printf("\nRunning Part 2-B...\n");
            system("./part2b/test_program");
            break;

        case 6:
            setupMenu();
            break;

        case 7:
            printf("\nReturning to Main Menu...\n");
            break;

        default:
            printf("\nInvalid choice.\n");
    }
}


/* =========================
   SIMULATION STATISTICS
   ========================= */

void showStatistics(void)
{
    printf("\n---------- SIMULATION STATISTICS ----------\n");
    printf("Simulation Seed: %u\n", simulationSeed);
    printf("Result files are saved inside the part folders.\n");
    printf("-------------------------------------------\n");
}
