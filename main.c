#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// External function prototypes
void run_part_1a();
void run_part_1b();
void run_part_1c();
void run_part_2a();
void run_part_2b();

void setup_menu(unsigned int *seed) {
    int choice;
    printf("\n--- SETUP SUBMENU ---\n");
    printf("1. Set Seed Value\n");
    printf("2. Return to Main Menu\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        printf("Enter Seed Value: ");
        scanf("%u", seed);
        srand(*seed);
        printf(">>> Success: Seed updated to %u!\n", *seed);
    }
}

void start_simulation_menu() {
    int choice;
    printf("\n--- SELECT SIMULATION PART ---\n");
    printf("1. Part 1-A\n2. Part 1-B\n3. Part 1-C\n4. Part 2-A\n5. Part 2-B\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: run_part_1a(); break;
        case 2: run_part_1b(); break;
        case 3: run_part_1c(); break;
        case 4: run_part_2a(); break;
        case 5: run_part_2b(); break;
        default: printf(">>> Invalid Selection!\n");
    }
}

void view_statistics() {
    FILE *file = fopen("simulation_results.txt", "r");
    if (file == NULL) {
        printf("\n>>> No previous simulation results found.\n");
        return;
    }
    
    char ch;
    printf("\n=== PAST SIMULATION STATISTICS ===\n");
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
    printf("\n===================================\n");
}

int main() {
    int choice;
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);

    while(1) {
        printf("\n====================================\n");
        printf("   NAVAL BATTLE SIMULATOR MAIN MENU\n");
        printf("====================================\n");
        printf("1. Start Simulation\n");
        printf("2. Setup\n");
        printf("3. View Instructions\n");
        printf("4. Simulation Statistics\n");
        printf("5. Exit\n");
        printf("Enter option: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: start_simulation_menu(); break;
            case 2: setup_menu(&seed); break;
            case 3: 
                printf("\n--- INSTRUCTIONS ---\n");
                printf("1. Set seed value in Setup for reproducible runs.\n");
                printf("2. Select 'Start Simulation' to execute battle parts.\n");
                break;
            case 4: view_statistics(); break;
            case 5: exit(0);
            default: printf(">>> Invalid choice!\n");
        }
    }
    return 0;
}
