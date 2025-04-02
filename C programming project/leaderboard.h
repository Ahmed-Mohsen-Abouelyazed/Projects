#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PLAYERS 100
#define MAX_NAME_LENGTH 50

typedef struct {
    int id; // Unique ID for each player
    char playerName[MAX_NAME_LENGTH]; // Player's name
    int score; // Player's score
} ScoreEntry;

typedef struct {
    char gameName[MAX_NAME_LENGTH]; // Name of the game
    ScoreEntry scores[MAX_PLAYERS]; // Array of score entries
    int scoreCount; // Current number of scores in the leaderboard
} GameLeaderboard;

// Function declarations
void initializeLeaderboard(GameLeaderboard *leaderboard, const char *gameName);
void saveLeaderboard(GameLeaderboard *leaderboard, const char *filename);
void loadLeaderboard(GameLeaderboard *leaderboard, const char *filename);
int addScore(GameLeaderboard *leaderboard, const char *playerName, int score);
void displayLeaderboard(const GameLeaderboard *leaderboard);

#endif // LEADERBOARD_H