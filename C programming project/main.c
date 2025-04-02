#include <stdio.h>
#include <string.h>
#include "pacman.h"
#include "leaderboard.h"

void displayMenu() {
    printf("Welcome to the Game Hub!\n");
    printf("1. Play Pacman\n");
    printf("2. Exit\n");
    printf("Choose an option: ");
}

void login(char *username) {
    printf("Enter your username: ");
    scanf("%s", username);
}

int main() {
    GameLeaderboard leaderboard;
    char username[MAX_NAME_LENGTH];
    char filename[] = "score.txt";

    // Load existing leaderboard
    strncpy(leaderboard.gameName, "Pacman", MAX_NAME_LENGTH);
    loadLeaderboard(&leaderboard, filename);

    // User login
    login(username);

    int choice;
    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                runPacmanGame(); // Call the Pacman game function
                // After the game ends, save the score
                addScore(&leaderboard, username, score); // Use the actual score from the game
                saveLeaderboard(&leaderboard, filename); // Save updated leaderboard
                displayLeaderboard(&leaderboard); // Display the leaderboard
                break;
            case 2:
                printf("Exiting the game hub. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}