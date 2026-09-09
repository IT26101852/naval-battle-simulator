#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global settings
int global_seed = -1; // -1 means default (time-based)

void display_instructions() {
    printf("\n--- INSTRUCTIONS ---\n");
    printf("1. Select setup options to configure simulation properties.\n");
    printf("2. Run individual parts (1-A, 1-B, etc.) from the Start Simulation menu.\n");
    printf("3. View saved output files under Simulation Statistics.\n");
    printf("\nPress Enter to return...");
    getchar(); getchar();
}

void setup_submenu() {
    int choice = 0;
    while (choice != 4) {
        printf("\n=== SETUP SUBMENU ===\n");
        printf("1. Battleship Properties\n");
        printf("2. Escort Settings\n");
        printf("3. Set Seed Value (Current: %s)\n", (global_seed == -1) ? "Random/Time" : "Manual");
        printf("4. Return to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // TODO: Link Battleship setup logic
                printf("Battleship properties menu...\n");
                break;
            case 2:
                // TODO: Link Escort settings logic
                printf("Escort settings menu...\n");
                break;
            case 3:
                printf("Enter integer seed value: ");
                scanf("%d", &global_seed);
                srand(global_seed);
                printf("Seed set to %d successfully!\n", global_seed);
                break;
            case 4:
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void start_simulation_menu() {
    // Apply seed before simulation
    if (global_seed == -1) {
        srand(time(NULL));
    } else {
        srand(global_seed);
    }

    int choice = 0;
    printf("\n=== START SIMULATION ===\n");
    printf("1. Run Part 1-A\n");
    printf("2. Run Part 1-B\n");
    printf("3. Run Part 1-C\n");
    printf("4. Run Part 2-A\n");
    printf("5. Run Part 2-B\n");
    printf("6. Back to Main Menu\n");
    printf("Select Part to run: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // TODO: Call Part 1-A function
            printf("Running Part 1-A...\n");
            break;
        case 2:
            // TODO: Call Part 1-B function
            break;
        case 3:
            // TODO: Call Part 1-C function
            break;
        case 4:
            // TODO: Call Part 2-A function
            break;
        case 5:
            // TODO: Call Part 2-B function
            break;
        case 6:
            break;
        default:
            printf("Invalid selection.\n");
    }
}

void view_statistics() {
    // TODO: Read and display .txt result files
    printf("\n--- SIMULATION STATISTICS ---\n");
    printf("Loading .txt result files...\n");
}

int main() {
    int choice = 0;
    while (1) {
        printf("\n=================================\n");
        printf("   NAVAL BATTLE SIMULATOR        \n");
        printf("=================================\n");
        printf("1. Start Simulation\n");
        printf("2. Setup / Configuration\n");
        printf("3. View Instructions\n");
        printf("4. Simulation Statistics\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                start_simulation_menu();
                break;
            case 2:
                setup_submenu();
                break;
            case 3:
                display_instructions();
                break;
            case 4:
                view_statistics();
                break;
            case 5:
                printf("Exiting simulator. Goodbye!\n");
                return 0;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}
