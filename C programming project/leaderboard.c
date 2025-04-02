#include "leaderboard.h"

static int nextPlayerId = 1; // Static variable to generate unique player IDs

void initializeLeaderboard(GameLeaderboard *leaderboard, const char *gameName) {
    strncpy(leaderboard->gameName, gameName, MAX_NAME_LENGTH);
    leaderboard->scoreCount = 0;
}

void saveLeaderboard(GameLeaderboard *leaderboard, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    for (int i = 0; i < leaderboard->scoreCount; i++) {
        fprintf(file, "%d %s %d\n", leaderboard->scores[i].id, leaderboard->scores[i].playerName, leaderboard->scores[i].score);
    }

    fclose(file);
}

void loadLeaderboard(GameLeaderboard *leaderboard, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file for reading");
        return;
    }

    leaderboard->scoreCount = 0;

    while (fscanf(file, "%d %s %d", &leaderboard->scores[leaderboard->scoreCount].id, leaderboard->scores[leaderboard->scoreCount].playerName, &leaderboard->scores[leaderboard->scoreCount].score) != EOF) {
        leaderboard->scoreCount++;
    }

    fclose(file);
}

int addScore(GameLeaderboard *leaderboard, const char *playerName, int score) {
    if (leaderboard->scoreCount >= MAX_PLAYERS) {
        return -1; // Leaderboard is full
    }

    leaderboard->scores[leaderboard->scoreCount].id = nextPlayerId++;
    strncpy(leaderboard->scores[leaderboard->scoreCount].playerName, playerName, MAX_NAME_LENGTH);
    leaderboard->scores[leaderboard->scoreCount].score = score;
    leaderboard->scoreCount++;
    return 0; // Success
}

void displayLeaderboard(const GameLeaderboard *leaderboard) {
    printf("Leaderboard for %s:\n", leaderboard->gameName);
    printf("Rank\tID\tPlayer Name\tScore\n");
    for (int i = 0; i < leaderboard->scoreCount; i++) {
        printf("%d\t%d\t%s\t%d\n", i + 1, leaderboard->scores[i].id, leaderboard->scores[i].playerName, leaderboard->scores[i].score);
    }
}