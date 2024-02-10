/**
 * @file snake.c
 * @author Martin Luthi
 * @brief   Fonctions pour le jeu snake
 * @version 0.1
 * @date 2024-02-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "snake.h"

int isFoodOnSnake() {
    for (int i = 0; i < snake.length; i++) {
        if (food.x == snake.x[i] && food.y == snake.y[i]) {
            return 1;
        }
    }
    return 0;
}

void spawnFood(){
    // Init the position of the food and make sure it is not on the snake or outside the grid
    do {
        food.x = rand() % (GRID_SIZE - 2);
        food.y = rand() % (GRID_SIZE - 2);
    } while (isFoodOnSnake() || (food.x == snake.x[0] && food.y == snake.y[0]));
}