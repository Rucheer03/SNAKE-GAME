# Snake Game 🐍🐍🐍

### Created by:
- **Rucheer Patel** 
- **Preet Patel** 

---

## Introduction

The Snake Game is a classic arcade game where players control a snake that grows longer as it eats food, with the player’s score increasing accordingly. The game ends if the snake crashes into either its own body or the boundaries of the play area, at which point the final score is displayed.

---

## Objective

The goal of the game is to control the snake to eat as much food as possible without colliding with the walls or the snake's body. Each time the snake eats food, it grows, and the player's score increases.

---
## Requirements

### Platform

- **Windows** or **macOS**.

### Compiler

- A C++ compiler that supports the C++11 standard or later:
  - **GCC** (MinGW) for Windows.
  - **MSVC** for Windows.
  - **Clang** for macOS.

### Libraries

- **conio.h** (Windows only) for input handling functions like `_kbhit()` and `_getch()`.
- **windows.h** (Windows only) for `Sleep()` to control the game's speed.
- **unistd.h** (macOS only) for `sleep()`.

## How to Compile and Run

### On Mac

#### Using Clang:
1. Open the Terminal and navigate to the folder where the game file is saved.
2. Use the following command to compile the code:
   ```bash
   clang++ snakegame_mac.cpp -o snakegame_mac
3. Run the compiled game:
   ```bash
   ./snakegame_mac
### On Windows

#### Using GCC (MinGW):

1. Open the **Command Prompt** (or **PowerShell**) in the folder where the game file is saved.
2. Use the following command to compile the code:
   ```bash
   g++ snakegame_windows.cpp -o snakegame_windows.exe
3. Run the compiled game:
   ```bash
   ./snakegame_windows.exe

## Display of the Game
The game is displayed in the console using ASCII characters:
```
--------------------------------------
             SNAKE GAME
--------------------------------------
Controls: 'W' - Up | 'S' - Down | 'A' - Left | 'D' - Right | 'X' - Exit
--------------------------------------
########################################
#                                      #
#                                      #
#                                      #
#                                      #
#                                      #
#                                      #
#           $                          #
#                                      #
#                                      #
#                                      #
#                  0                   #
#                  0                   #
#                  0                   #
#                                      #
#                                      #
#                                      #
#                                      #
#                                      #
#                                      #
#                                      #
########################################
Score: 00
Highest score : 00
```
## Key Features

- **Cross-Platform Support**:
  - The game is available for both **Windows** and **Mac**. One version is tailored for Mac, and the other for Windows, allowing players to enjoy the game regardless of their operating system.

- **Terminal Interface**:
  - The game runs in the terminal/console, making it a simple, text-based game.

- **Game Grid and Boundaries**:
  - The game is played in a grid of size 40x20 (width x height), with boundaries represented by `#` characters.
  
- **Basic Visuals**:
  - The snake is represented by the character 0 and food by the $ symbol.
  - The grid and other visuals are drawn at each frame by clearing the console with system("clear"); (on Linux/macOS).

- **Snake Movement**:
  - The snake starts in the center of the grid and can move up, down, left, or right using the **W, A, S, D** keys.

- **Snake Growth**:
  - The snake grows each time it eats food, and the snake’s body follows its head. New segments are added to the tail.

- **Food**:
  - The food is randomly placed on the grid and represented by the `$` symbol. The snake eats the food, growing longer and gaining points.

- **Score System**:
  - Each time the snake eats food, the player earns 10 points, which is tracked and displayed in real-time.
  - After game ends highest score of player also apppear on the screen
  
- **Game Over and Restart**:
  - The game ends if the snake collides with itself or the grid boundaries. The player is prompted to play again.

- **Dynamic Difficulty**:
  - The game speed can be adjusted (controlled by `usleep(200000);`), making it more challenging as the game progresses.

---
## Data structure used


### 1. **Class: Food**

- **Attributes**:
  - `int x, y`: The coordinates (position) of the food on the grid.
  
- **Constructor**:
  - `Food(int gridWidth, int gridHeight)`: Initializes the food's position by calling the respawn method.
  
- **Methods**:
  - `void respawn(int gridWidth, int gridHeight, const vector<pair<int, int>>& snakeBody)`: Spawns food in a random position, avoiding overlap with the snake’s body.

---

### 2. **Class: Snake**

- **Attributes**:
  - `vector<pair<int, int>> body`: The snake’s body, with the head at the beginning of the vector and the tail at the end.
  - `Direction dir`: The current direction of movement (UP, DOWN, LEFT, RIGHT, STOP).
  
- **Constructor**:
  - `Snake()`: Sets the snake’s initial state (centered, stopped).
  
- **Methods**:
  - `void move()`:
    - Moves the snake in the current direction.It updates the snake’s head position based on its direction and then shifts the body.
    - The snake’s body follows the head, and the tail segment is removed (i.e., pop_back is used) to simulate movement.


  - `void grow()`: Adds a new segment to the snake’s body using push_back.
  - `bool hasCollided()`: Checks for collisions with itself or the boundaries.

---

### 3. **Class: Game**

- **Attributes**:
  - `Snake snake`: Instance of the Snake class.
  - `Food food`: Instance of the Food class.
  - `int score`: Current score.
  - `bool gameOver`: Flag indicating if the game is over.
  
- **Constructor**:
  - `Game()`: Initializes the game state, places food, and sets the score to 0.

- **Methods**:
  - `void draw()`: Draws the grid, snake, and food.It also prints the current score.
  - `void input()`: It checks if any key is pressed using `kbhit()` and then updates the snake’s direction accordingly.

  - `void logic()`: 
    - This part handles the game logic
    - It checks if the snake eats the food. If it does, the score is increased, the snake grows, and a new food is spawned at a random position.
    - The snake then moves, and the game checks for any collisions (either with the snake’s body or the grid boundaries).
  - `void run()`: 
    - This method controls the flow of the game
    - It enters a loop where it repeatedly calls `draw()`, `input()`, and `logic()` to run the game.
    - The game continues until the gameOver flag is set to true.
    - It also disables and enables terminal echo to handle keypresses in real time.
  - `int getScore()`: Returns the current score.

---
## Other data structure used

### 1. `enum Direction`

- **Usage**:  
  The `Direction` enum represents the current direction in which the snake is moving. The possible directions are:
  - `STOP`
  - `UP`
  - `DOWN`
  - `LEFT`
  - `RIGHT`

- **Reason for usage**:  
  The `enum` type defines a set of named integer constants, improving the code's readability and ensuring that the snake’s movement is restricted to a predefined set of values.

### 2. `std::vector<std::pair<int, int>>`

- **Used in**:  
  - `Snake::body`
  - `Food::respawn()`
  - Checking for collisions

- **Purpose**:  
  This vector stores the positions of the snake's body segments on the grid. Each segment is represented as a pair of coordinates (x, y), where `x` is the column position and `y` is the row position.

- **Details**:  
  The snake's body grows when the snake eats food. The new body segment is added to the front of the vector, and the tail is removed (using `pop_back`) to simulate movement.

## Randomization

### `void srand(time(0))`

- **Usage**:  
  The `srand(time(0))` function call is used to seed the random number generator with the current time. This ensures that the random numbers generated in the game (like the position of food) are different each time the game is played.

- **Reason for usage**:  
  `srand` is used for random number generation, and `time(0)` provides a constantly changing seed value (the current time in seconds).
---
## Code Structure and Flow

1. **Input Handling**: The game uses the `kbhit` function to detect if a key is pressed without blocking, allowing real-time movement.
2. **Game Loop**: The game runs in a continuous loop, where:
   - The game grid is drawn.
   - User input is checked for movement.
   - The snake’s position is updated.
   - Speed is adjusted using `usleep()`.
3. **Display**: The grid is cleared and redrawn each frame, showing the snake's current position and food.

---

## Improvements and Features to Add

1. **Speed Variability**: Increase the game speed as the score increases to make it more challenging.
2. **Additional Food Types**: Introduce power-ups that temporarily enhance the snake’s speed or provide other effects.

---

## Conclusion

This Snake game implementation demonstrates basic game development using C++ concepts like Object-Oriented Programming (OOP), handling user input, collision detection, and game flow management. The game is simple but engaging, with plenty of opportunities to expand and enhance the gameplay.

---



## License

This project is open source. Feel free to modify and distribute it as you like.



