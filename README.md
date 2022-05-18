# Solitude

## Installation instructions:

1. Install [MobaXterm](https://mobaxterm.mobatek.net/download-home-edition.html) and start the server (should start on open)
2. Open a linux terminal. I used Ubuntu (wsl)
3. Install g++: `sudo apt-get install g++`
4. Install make: `sudo apt-get install make`
5. Install SFML: `sudo apt-get install sfml`
6. Install pulseaudio: `sudo apt-get install pulseaudio`
7. Enter the following command: `export DISPLAY=:0.0`
8. In the main directory run `make` to compile the executable
9. Start pulseaudio: `pulseaudio --start` (pulseaudio must be on everytime you start the game, otherwise it will give you endless errors)
10. Finally start the game with `./game.app`

## Game controls:

Move player using arrow keys\
Swing sword with space bar\
Pause with escape button
