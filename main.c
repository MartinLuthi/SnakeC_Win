/**
 * @file main.c
 * @author Martin Luthi
 * @brief This is the main file for the game "Snake".
 * @version 0.1
 * @date 2024-02-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// Include
#include <time.h>       // Time
#include <windows.h>    // Windows
#include "appWindows.h" // Windows functions

int main() {
    // Set the seed of the random number generator
    srand(time(NULL));

    // Create the window
    WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOWNORMAL);
    
    return 0;
}
