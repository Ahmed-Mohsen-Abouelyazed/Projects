#ifndef PACMAN_H
#define PACMAN_H

// Game constants
#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'
#define FOOD '.'
#define EMPTY ' '
#define DEMON 'X'

// Global variables
int res = 0; // Result of the game (0: running, 1: lost, 2: won)
int score = 0; // Player's score
int pacman_x, pacman_y; // Pacman's position
char board[HEIGHT][WIDTH]; // Game board
int food = 0; // Total food items
int curr = 0; // Current food eaten

// Function to run the Pacman game
void runPacmanGame();

// Function to initialize the game board and variables
void initialize();

// Function to draw the game board
void draw();

// Function to move Pacman based on user input
void move(int move_x, int move_y );

#endif // PACMAN_H