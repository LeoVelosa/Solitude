# Solitude

# What is Solitude

Solitude is a roguelike game where your goal is to reach and kill the final boss.\
Every time you die you must restart.

# Storyline

An evil spirit has taken over the kingdom and one lone knight has survived. The knight must venture through the demolished kingdom that has been overrun by monsters and must destroy everything that stands in the way of taking back the kingdom. 

## Installation instructions:

1. Clone repository
1. Install [MobaXterm](https://mobaxterm.mobatek.net/download-home-edition.html) and start the server (should start on open)
2. Open any Unix terminal. I used Ubuntu (wsl)
3. Run 'sudo apt-get update'
4. Install g++: `sudo apt-get install g++`
5. Install make: `sudo apt-get install make`
6. Install SFML: `sudo apt-get install libsfml-dev`
7. Enter the following command: `export DISPLAY=:0.0`
8. Go to the directory that you cloned the repository, run `make` to compile an executable
10. Finally start the game with `./game.app`

## Game controls:

Move player using arrow keys\
Swing sword with space bar\
Pause with escape button
