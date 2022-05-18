# Solitude

## Installation instructions:

1. Go to the github page and clone the repository
2. Download MobaXterm home edition installer for windows
3. Install MobaXterm and start the server (should start on open)
4. Open a linux terminal. I used Ubuntu (wsl)
5. sudo apt-get update
6. Install g++: sudo apt-get install g++
7. Install make: sudo apt-get install make
8. Install SFML: sudo apt-get install sfml
9. Install pulseaudio: sudo apt-get install pulseaudio
10. Go to the directory that you cloned the repository
11. Enter the following command: export DISPLAY=:0.0
12. Start pulseaudio: pulseaudio --start (pulseaudio must be on everytime you start the game, otherwise it will give you endless errors)
13. Run make to compile the executable
14. Finally start the game with ./game.app

## Game controls:

Move player using arrow keys\
Swing sword with space bar\
Pause with escape button
