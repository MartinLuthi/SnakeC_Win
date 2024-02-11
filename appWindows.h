/**
 * @file appWindows.h
 * @author Martin Luthi
 * @brief   Functions for the windows of the game
 * @version 0.1
 * @date 2024-02-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef APPWINDOWS_H
#define APPWINDOWS_H

// Include
#include <windows.h>
#include <stdio.h>      // Standard input/output
#include <stdlib.h>     // Standard library

// Define
#define GRID_SIZE 20                            // The size of the grid
#define CELL_SIZE 20                            // The size of the cells
#define WINDOW_HEIGHT (GRID_SIZE * CELL_SIZE)   // The height of the window
#define WINDOW_WIDTH (GRID_SIZE * CELL_SIZE)    // The width of the window
#define ID_TIMER 1                              // The ID of the timer
#define SPEED_INIT 100                          // The initial speed of the snake in milliseconds

// Global variables
extern struct Snake snake;  // Instance of the snake
extern struct Food food;    // Instance of the food

// Prototypes

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);
void WM_keydown(WPARAM w_param);
void WM_create(HWND hwnd);
void WM_time(HWND hwnd);
void WM_paint(HWND hwnd);
void InitializeBrushes();
void DestroyBrushes();
void ResetWindows(HWND hwnd);

#endif // APPWINDOWS_H