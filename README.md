# RaylibTicTacToe

## Overview

This project is a simple Tic Tac Toe game built using the [Raylib](https://www.raylib.com/) library. It showcases basic game development techniques such as handling user input, rendering graphics, and implementing game logic.

## Dependencies

- [Raylib](https://www.raylib.com/): A simple and easy-to-use library to enjoy videogames programming. This project uses Raylib version 5.0 for Windows (64-bit) with MSVC 16.
- [CMake](https://cmake.org/): A cross-platform tool designed to build, test, and package software.

## Setup and Build Instructions

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/GitVladlen/RaylibTicTacToe.git
   cd RaylibTicTacToe
   ```

2. **Build the Project:**

   Run the provided batch file to create a build directory and generate project files:

   ```bash
   build.bat
   ```

3. **Open the Project in Visual Studio:**

   After running the batch file, a Visual Studio solution file (`RaylibTicTacToe.sln`) will be created in the `build` directory. Open this file with Visual Studio.

4. **Build and Run:**

   In Visual Studio, set the build configuration to `Release` or `Debug` as desired and build the solution. After building, run the executable to start the Tic Tac Toe game.

## Usage

- **Controls:**
  - `Left Mouse Button (LMB)`: Click on a cell to make a move.
  - `Right Mouse Button (RMB)`: Click to reset the game.
  - `Space/N/R`: Press any of these keys to reset the game.

- **Gameplay:**
  - The game is designed for two players. Player X and Player O take turns to mark the cells in a 3x3 grid.
  - The first player to align three of their marks horizontally, vertically, or diagonally wins the game.
  - If the grid is filled and no player has aligned three marks, the game is a draw.