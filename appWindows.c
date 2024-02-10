/**
 * @file appWindows.c
 * @author Martin Luthi
 * @brief   Functions for the windows of the game
 * @version 0.1
 * @date 2024-02-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// Includes
#include "appWindows.h"
#include "snake.h"

// Global variables
struct Snake snake;  // Instance of the snake
struct Food food;    // Instance of the food

// Global brushes
HBRUSH hBackgroundBrush;
HBRUSH hSnakeBrush;
HBRUSH hFoodBrush;

/**
 * @brief                   The main function of the game
 * 
 * @param hInstance         The handle to the current instance of the application
 * @param hPrevInstance     The handle to the previous instance of the application
 * @param lpCmdLine         The command line for the application
 * @param nCmdShow          The display options
 * @return int              The exit code
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Enregistrement de la classe de fenêtre
    WNDCLASS windowClass = { 0 };               // Create a window class
    windowClass.lpfnWndProc = WndProc;          // Set the window procedure
    windowClass.hInstance = hInstance;          // Set the instance
    windowClass.lpszClassName = "SnakeGame";    // Set the class name

    // Register the window class with the system and return the class atom for use in creating windows.
    if (!RegisterClass(&windowClass)) {
        MessageBoxW(NULL, L"The window class could not be registered.", L"Error", MB_ICONERROR);
        return 1;   // Return 1 if the window class could not be registered
    }

    // Set the locale of the thread
    SetThreadLocale(LOCALE_SYSTEM_DEFAULT);

    // Create the window
    HWND hwnd = CreateWindow(
        "SnakeGame", "Snake",                           
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,       // Window style (remove WS_THICKFRAME for fixed size)
        CW_USEDEFAULT, CW_USEDEFAULT,                  // Initial position (x, y)
        WINDOW_WIDTH, WINDOW_HEIGHT,                   // Initial size (width, height)
        NULL, NULL, hInstance, NULL);                  // Parent window, menu, instance, param

    // If the window could not be created, return 1
    if (!hwnd) {
        MessageBoxW(NULL, L"The window could not be created.", L"Error", MB_ICONERROR);
        return 1;
    }

    // Show the window
    ShowWindow(hwnd, nCmdShow);

    

    // Message loop (event loop) for the window
    MSG msg;   // Create a message structure
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);    // Translate the message
        DispatchMessage(&msg);     // Dispatch the message
    }

    return msg.wParam;
}

/**
 * @brief                   The window procedure
 * 
 * @param hwnd              The handle to the window
 * @param message           The message
 * @param w_param           The first message parameter
 * @param l_param           The second message parameter
 * @return LRESULT          The result of the message processing
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
    switch (message) {
        
    // Create the window
    case WM_CREATE:
        WM_create(hwnd);
        break;

    // Handle the destroy message
    case WM_DESTROY:
        // Clean up brushes
        DestroyBrushes();

        // Post a quit message to the window
        PostQuitMessage(0);
        break;
    
    // Handle the paint message
    case WM_PAINT:
        WM_paint(hwnd);
        break;

    // Handle the timer message
    case WM_TIMER:
        // Move the snake and check if the game is over
        WM_time(hwnd);   
        break;

    // Handle the key down message
    case WM_KEYDOWN:
        WM_keydown(w_param);
        break;

    // For all other messages, call the default window procedure
    default:
        return DefWindowProc(hwnd, message, w_param, l_param);
    }

    return 0;
}

/**
 * @brief                   Create the window
 * 
 * @param hwnd              The handle to the window
 */
void WM_create(HWND hwnd) {
    // Init the initial direction of the snake
    snake.direction = 0;        // Up
    snake.length = 1;           // Length of the snake
    snake.x[0] = GRID_SIZE / 2; // Initial x position of the snake Center of the grid
    snake.y[0] = GRID_SIZE / 2; // Initial y position of the snake Center of the grid
    snake.speed = SPEED_INIT;   // Speed of the snake
    snake.score = 0;            // Score of the snake
    snake.gameOver = FALSE;     // Game over flag

    // Init the position of the food and make sure it is not on the snake or outside the grid
    spawnFood();

    // Initialize brushes
    InitializeBrushes();

    // Start the timer for the game
    SetTimer(hwnd, 1, snake.speed, NULL);  // 100 millisecondes
}

/**
 * @brief                   Paint the window
 * 
 * @param hwnd              The handle to the window
 */
void WM_paint(HWND hwnd){

    // Get the device context
    PAINTSTRUCT ps;                     // Create a paint structure
    HDC hdc = BeginPaint(hwnd, &ps);    // Get the device context

    // Fill the background with the black brush
    RECT rectBackground;
    rectBackground.left = 0;
    rectBackground.top = 0;
    rectBackground.right = WINDOW_WIDTH;
    rectBackground.bottom = WINDOW_HEIGHT;

    // Fill the rectangle with the black brush
    FillRect(hdc, &rectBackground, hBackgroundBrush);

    // Drawing the food (green color)
    RECT rectFood;
    rectFood.left = food.x * CELL_SIZE;
    rectFood.top = food.y * CELL_SIZE;
    rectFood.right = (food.x + 1) * CELL_SIZE;
    rectFood.bottom = (food.y + 1) * CELL_SIZE;

    FillRect(hdc, &rectFood, hFoodBrush);

    // Drawing the snake
    for (int i = 0; i < snake.length; i++) {
        // Draw the new position of the snake with the snake brush
        RECT rectNewSnake;
        rectNewSnake.left = snake.x[i] * CELL_SIZE;
        rectNewSnake.top = snake.y[i] * CELL_SIZE;
        rectNewSnake.right = (snake.x[i] + 1) * CELL_SIZE;
        rectNewSnake.bottom = (snake.y[i] + 1) * CELL_SIZE;

        FillRect(hdc, &rectNewSnake, hSnakeBrush);
    }

    // End the painting and release the device context
    EndPaint(hwnd, &ps);
}


/**
 * @brief                   Move the snake and check if the game is over
 * 
 * @param hwnd              The handle to the window
 */
void WM_time(HWND hwnd){
    
    // Move the body of the snake with the current direction
    for (int i = snake.length - 1; i > 0; i--) {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    // Move the head of the snake with the current direction
    switch (snake.direction) {
    case 0: // Up
        snake.y[0]--;
        break;
    case 1: // Right    
        snake.x[0]++;
        break;
    case 2: // Down
        snake.y[0]++;
        break;
    case 3: // Left
        snake.x[0]--;
        break;
    }

    // Check if the snake is out of the grid
    if (snake.x[0] < 0 || snake.x[0] >= GRID_SIZE || snake.y[0] < 0 || snake.y[0] >= GRID_SIZE) {
        
        // Stop the timer
        KillTimer(hwnd, ID_TIMER);  

        // Reset the game
        ResetWindows(hwnd);
    }

    // Check if the snake eats itself
    for (int i = 1; i < snake.length; i++) {
        if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
            // stop the timer
            KillTimer(hwnd, ID_TIMER);

            // Reset the game
            ResetWindows(hwnd);
        }
    }

    // Check if the snake eats the food
    if (snake.x[0] == food.x && snake.y[0] == food.y) {
        // Increase the length of the snake
        snake.length++;

        // Move the tail of the snake to the new position
        snake.x[snake.length - 1] = snake.x[snake.length - 2];
        snake.y[snake.length - 1] = snake.y[snake.length - 2];

        // Change the position of the food
        spawnFood();

        // Increase the score
        snake.score++;
    }

    // Update the window
    InvalidateRect(hwnd, NULL, TRUE);
}


/**
 * @brief                   Handle the key down message (arrow keys) to change the direction of the snake
 *                          The direction of the snake cannot be changed to the opposite direction
 * 
 * @param w_param           The first message parameter
 */
void WM_keydown(WPARAM w_param) {
    switch (w_param) {
    case VK_UP:
        if (snake.direction != 2)
            snake.direction = 0;  // Up
        break;
    case VK_RIGHT:
        if (snake.direction != 3)
            snake.direction = 1;  // Right
        break;
    case VK_DOWN:
        if (snake.direction != 0)
            snake.direction = 2;  // Down
        break;
    case VK_LEFT:
        if (snake.direction != 1)
            snake.direction = 3;  // Left
        break;
    default:
        break;
    }
}

/**
 * @brief                   Reset the window
 * 
 * @param hwnd              The handle to the window
 */
void ResetWindows(HWND hwnd){
    // Initialize the window again
    WM_create(hwnd);
}

/**
 * @brief                   Initialize the brushes
 */
void InitializeBrushes() {
    // Create a brush to fill the background (black)
    hBackgroundBrush = CreateSolidBrush(RGB(0, 0, 0));

    // Create a brush to draw the snake (red color)
    hSnakeBrush = CreateSolidBrush(RGB(255, 0, 0));

    // Create a brush to draw the food (green color)
    hFoodBrush = CreateSolidBrush(RGB(0, 255, 0));
}

/**
 * @brief                   Destroy the brushes
 */
void DestroyBrushes() {
    // Delete brushes
    DeleteObject(hBackgroundBrush);
    DeleteObject(hSnakeBrush);
    DeleteObject(hFoodBrush);
}