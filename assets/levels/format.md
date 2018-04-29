# Pacman level formt

### Size
The first line specifies the map size  
"[Width] [Height]"

### Tile types
Wall: '#'  
Floor: '-'

Floor with pellet: '.'  
Floor with energizer: 'E'

Pacman spawn location: 'P'

Ghost door: 'D'  
Ghost floor: 'H'  
Ghost spawn location: 'G'

Ghost scatter tiles: 'S', 'B', 'C', 'I'

Teleporter pairs: 0-9

### Ghost house construction
The ghost house is where the ghost spawn and go to respawn when eaten by Pacman.  
The ghost spawn (G) marks the exact thile they wil spawn on.  
The floor (H) is similar to the normal floor exept is causing live ghosts to move upwards.  
The door (D) is a visible barrier only the ghost can go through. The door should always be above the ghost's spawn point

### Ghost scatter tiles
The scatter tile is the tile a ghost will try to reach during scatter mode.  
The tile should be placed outside the map in a way that the ghost will travel in an endless loop while trying to reach it.

### Teleporters
Teleporters are defined as pairs.  
0 and 1 is a pair. 2 and 3 is a pair etc.

## Example level
```
28 36
# # S # # # # # # # # # # # # # # # # # # # # # # B # #
# # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # # # # # # # # # # # # #
# . . . . . . . . . . . . # # . . . . . . . . . . . . #
# . # # # # . # # # # # . # # . # # # # # . # # # # . #
# E # # # # . # # # # # . # # . # # # # # . # # # # E #
# . # # # # . # # # # # . # # . # # # # # . # # # # . #
# . . . . . . . . . . . . . . . . . . . . . . . . . . #
# . # # # # . # # . # # # # # # # # . # # . # # # # . #
# . # # # # . # # . # # # # # # # # . # # . # # # # . #
# . . . . . . # # . . . . # # . . . . # # . . . . . . #
# # # # # # . # # # # # - # # - # # # # # . # # # # # #
# # # # # # . # # # # # - # # - # # # # # . # # # # # #
# # # # # # . # # - - - - - - - - - - # # . # # # # # #
# # # # # # . # # - # # # D D # # # - # # . # # # # # #
# # # # # # . # # - # H H H H H H # - # # . # # # # # #
0 - - - - - . - - - # H H H G H H # - - - . - - - - - 1
# # # # # # . # # - # H H H H H H # - # # . # # # # # #
# # # # # # . # # - # # # # # # # # - # # . # # # # # #
# # # # # # . # # - - - - P - - - - - # # . # # # # # #
# # # # # # . # # - # # # # # # # # - # # . # # # # # #
# # # # # # . # # - # # # # # # # # - # # . # # # # # #
# . . . . . . . . . . . . # # . . . . . . . . . . . . #
# . # # # # . # # # # # . # # . # # # # # . # # # # . #
# . # # # # . # # # # # . # # . # # # # # . # # # # . #
# E . . # # . . . . . . . . . . . . . . . . # # . . E #
# # # . # # . # # . # # # # # # # # . # # . # # . # # #
# # # . # # . # # . # # # # # # # # . # # . # # . # # #
# . . . . . . # # . . . . # # . . . . # # . . . . . . #
# . # # # # # # # # # # . # # . # # # # # # # # # # . #
# . # # # # # # # # # # . # # . # # # # # # # # # # . #
# . . . . . . . . . . . . . . . . . . . . . . . . . . #
# # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # # # # # # # # # # # # # # # # # # # # # # # # # # #
C # # # # # # # # # # # # # # # # # # # # # # # # # # I
```