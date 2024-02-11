/**
 * @file snake.h
 * @author Martin Luthi
 * @brief   Fonctions pour le jeu snake
 * @version 0.1
 * @date 2024-02-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SNAKE_H
#define SNAKE_H

// Includes
#include "appWindows.h"

// Prototypes
void spawnFood();
int isFoodOnSnake();

// Structures
struct Snake {
    int x[GRID_SIZE * GRID_SIZE];
    int y[GRID_SIZE * GRID_SIZE];
    int length;
    int direction;  // 0: up, 1: right, 2: down, 3: left
    int speed;
    int score;
    int gameOver;
};

struct Food {
    int x;
    int y;
};


#endif // SNAKE_H