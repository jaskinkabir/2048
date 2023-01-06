# 2048

This is my implementation of the game 2048 in the Windows terminal using. It only works on Windows because it uses the `getch()` function to receive user input from the arrow keys. The board is displayed using a somewhat boneheaded approach of hardcoding the layout of the board into several strings representing each line, and then replacing the numbers within the cells with their correct values determined from the game logic. 

## How to play
First, download and launch 2048.exe. This file can be found in the releases tab on the right side of the repository screen. The game is played using the arrow keys as you would play the standard 2048 game. The object is to bring the value in one of the tiles up to 2048. You can use the arrow keys to move every tile as far as they can go in the direction you selected. If two tiles of the same value collide, they will be combined into a single tile with twice the value. Each movement creates a new tile with a value of either 2 or 4, and the game ends when all tiles have been filled and there are no more possible moves. 
