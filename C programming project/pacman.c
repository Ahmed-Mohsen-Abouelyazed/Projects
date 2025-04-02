#include "pacman.h"
#include "leaderboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // For _kbhit() and _getch()

// Function to initialize the game board and variables
void initialize() {
    // Set up the game board with walls and empty spaces
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || j == WIDTH - 1 || j == 0 || i == HEIGHT - 1) {
                board[i][j] = WALL; // Set walls
            } else {
                board[i][j] = EMPTY; // Set empty spaces
            }
        }
    }

    // Randomly place walls inside the game
    int count = 50;
    while (count != 0) {
        int i = (rand() % (HEIGHT - 1)); // Random row
        int j = (rand() % (WIDTH - 1)); // Random column

        if (board[i][j] != WALL && board[i][j] != PACMAN) {
            board[i][j] = WALL; // Place a wall
            count--;
        }
    }

    // Place demons randomly
    count = 10;
    while (count != 0) {
        int i = (rand() % (HEIGHT - 1)); // Random row
        int j = (rand() % (WIDTH - 1)); // Random column

        if (board[i][j] != WALL && board[i][j] != PACMAN) {
            board[i][j] = DEMON; // Place a demon
            count--;
        }
    }

    // Place Pacman at the center
    pacman_x = WIDTH / 2;
    pacman_y = HEIGHT / 2;
    board[pacman_y][pacman_x] = PACMAN;

    // Place food items on the board
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i % 2 == 0 && j % 2 == 0 && board[i][j] != WALL && board[i][j] != DEMON && board[i][j] != PACMAN) {
                board[i][j] = FOOD; // Place food
                food++;
            }
        }
    }
}

// Function to draw the game board
void draw() {
    // Clear the screen
    system("cls");

    // Draw the game board
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

// Function to move Pacman based on user input
void move(int move_x, int move_y) {
    int x = pacman_x + move_x;
    int y = pacman_y + move_y;

    // Check for wall collision
    if (board[y][x] != WALL) {
        if (board[y][x] == FOOD) {
            score++; // Increase score for eating food
            food--;
            curr++;
            if (food == 0) {
                res = 2; // Win condition
                return;
            }
        } else if (board[y][x] == DEMON) {
            res = 1; // Lose condition
        }

        // Update Pacman's position
        board[pacman_y][pacman_x] = EMPTY; // Clear old position
        pacman_x = x; // Update position
        pacman_y = y;
        board[pacman_y][pacman_x] = PACMAN; // Set new position
    }
}

// Function to run the Pacman game
void runPacmanGame() {
    initialize(); // Initialize the game board and variables

    while (res == 0) { // Game loop
        draw(); // Draw the current state of the game

        // Check for user input
        if (_kbhit()) {
            char ch = _getch(); // Get the character input
            switch (ch) {
                case 'w': // Move up
                    move(0, -1);
                    break;
                case 's': // Move down
                    move(0, 1);
                    break;
                case 'a': // Move left
                    move(-1, 0);
                    break;
                case 'd': // Move right
                    move(1, 0);
                    break;
                case 'q': // Quit the game
                    res = 1; // Set lose condition
                    break;
            }
        }
    }

    // Game over conditions
    if (res == 1) {
        printf("Game Over! You lost.\n");
    } else if (res == 2) {
        printf("Congratulations! You won!\n");
    }
}