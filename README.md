# Solitude

## Installation instructions:

1. Go to the github page and clone the repository
2. Download MobaXterm home edition installer for windows
3. Install MobaXterm and start the server (should start on open)
4. Open a linux terminal. I used Ubuntu (wsl)
5. sudo apt-get update
6. Install g++: sudo apt-get install g++
7. Install make: sudo apt-get install make
8. Install pulseaudio: sudo apt-get install pulseaudio
9. Go to the directory that you cloned the repository
10. Enter the following command: export DISPLAY=:0.0
11. Start pulseaudio: pulseaudio --start
12. Now enter: make
13. Finally start the game with ./game.app

## Game controls:

Move player using arrow keys\
Swing sword with space bar\
Pause with escape button
