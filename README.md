DUNGEON OF DOOM
===============

This is a port of Dungeon of Doom from BASIC to C. This was created for two reasons:

 - To brush up on C programming
 - To test the power of iterative refactoring

The initial stage, which has reached substantial completion, is a faithful reproduction of the original game. The next stage is to re-engineer the game into something more modern.

BUILDING
--------

The project currently has a simple makefile in the `src` directory. It currently builds on Linux for Linux and Windows targets. The executables are currently also written to the `src` directory. Issue #16 exists for restructuring the build system.

### Requirements
 - requires `SDL2` and `SDL2_ttf` and their development libraries
 - Windows build also requires MinGW, freetype, iconv, png, bz2 and z

### Targets
 - The end target names for Linux are `game_module`, `character_creator` and `dungeon_generator`.
 - The Windows target names match the Linux names plus the `.exe` extension.
 - `make all`, the default, builds all targets, `make all-linux` builds all Linux targets, `make all-windows` builds all Windows targets.

### Troubleshooting
 - Check the values for the `MINGW_GCC` and `MINGW_ROOT` Makefile macros if you get errors building for Windows targets

## INSTALLING

 - Issue #20 exists for creating installers / packages.

### Linux
Copy `dungeon_generator`, `character_creator`, `game_module` and the `fonts` directory to where you want to install the game.

### Windows
Copy the following files to the install location:
 - The game's own executables:
   - `dungeon_generator.exe`
   - `character_creator.exe`
   - `game_module.exe`
 - The `fonts` directory
 - The required MinGW libraries:
   - `libwinpthread-1.dll`
   - `SDL2.dll`
   - `SDL2_ttf.dll` and its dependencies:
     - `libfreetype-6.dll`
     - `libbz2-1.dll`
     - `libpng16-16.dll`
     - `zlib1.dll`
     - `libssp-0.dll`

## RUNNING THE PROGRAMS

### Dungeon Generator

#### Running

Run with `dungeon_generator` on Linux, `dungeon_generator.exe` on Windows. The program creates a new set of levels with each run. As this is still a "faithful recreation", you can not currently edit existing levels. (Issue #18 exists to rectify this.)

#### Controls

 - `h`: Draw Help
 - `a`: Move Up
 - `z`: Move Down
 - `n`: Move Left
 - `m`: Move Right
 - `1`: Wall
 - `2`: Vase
 - `3`: Chest
 - `4`: Idol
   - This is the final objective
 - `5`: Way in from previous level
 - `6`: Exit to next level
 - `7`: Trap
 - `8`: Safe Place
 - `9`: Guard
 - `0`: Erase
 - `s`: Save and start working Level
 - `f`: Quit

#### A note about saving levels.

Due to this still being a faithful recreation of the original game, saving levels is currently more complicated for the user than it should be. The original game was written for 8-bit computers with tape drives. Since the tape would be in a new position, the original program could create new files with the same file name without over-writing the old file. When the game was played, the tape would be rewound and the levels still loaded in order.

To work around this, copy `LEVEL` to a name containing the level number. When playing, copy the appropriate level file to `LEVEL` before loading the new level.

Issue #17 exists for cleaning up this situation.

### Character Creator

Run with `character_creator` on Linux, `character_creator.exe` on Windows. The program creates a new character with each run.

The first stage of the program sets the character's attributes. Subsequent stages are for purchasing items.

#### Controls

 - `A`: Move up one row
 - `Z`: Move down one row
 - `[SPACE]`: Move to next stage
 - `;`:
   - Increases selected attribute during the `Character Creation` stage
   - Buys the selected item in subsequent stages
 - `-`:
   - Decreases selected attribute during the `Character Creation` stage
   - Makes an offer for the selected item in subsequent stages

#### Character Classes

The character class currently only controls what items the character can buy / use.

##### Wanderer
 - The fallback if the character does not meet the criteria for any other classes
 - Can use all weapons except 2-Handed Swords, Broadswords and Flails
 - Can use all armour except Heavy Armour, Chain Armour and Gold Helmet
 - Can use all items from the Emporium except the Scrolls

##### Cleric
 - Requires an Intelligence greater than 6 and a Morality greater than 7
 - Can only use Daggers for weapons
 - Can use Heavy Robes, Headpieces and Shields for armour
 - Can use all items from the Emporium except the Scrolls and the Mystic Amulet

##### Mage
 - Requires an Intelligence greater than 8 and a Aura greater than 7
 - Can only use Daggers for weapons
 - Can only use Heavy Robes for armour
 - Can use all items from the Emporium

##### Warrior
 - Requires a Strength greater than 7, a Morality greater than 5 and a combined Strength and Vitality greater than 10
 - Can use all weapons except 2-Handed Swords
 - Can use all armour
 - Can only use Healing Salves and Potions from the Emporium

##### Barbarian
 - Requires a Strength greater than 8, a combined Vitality and Agility greater than 12 and a Morality lesser than 6
 - Can use all weapons
 - Can use all armour
 - Can only use Healing Salves and Potions from the Emporium

 - All classes can buy Torches, Healing Salves and Potions

#### A note about saving characters.

The program currently saves the character to a file named `HERO`. It is recommended that you copy this file to have a unique name.

### Game Module

Run with `game_module` on Linux, `game_module.exe` on Windows. The game loads the file `LEVEL` to read the current level and `HERO`. It is recommended that you copy the file containing the next level to the `LEVEL` file once you have finished loading the current level. (See the `A note about saving levels.` section of the `Dungeon Generator` notes and Issue #17 for more information.) The game ends either when the character dies or the character acquires the idol.

#### Controls

 - `a`: Attack
 - `c`: Cast spell
   - Requires an Aura greater than zero and either the Necronomicon or the Scrolls
   - See the `Spells` section for more information
 - `g`: Get the item in the square in front of the character
 - `p`: Drink a potion
   - If there are any left, uses a Potion and restores the character's strength to its initial value
   - If there are any left, uses a Healing Salve and restores the character's vitality to its initial value
 - `r`: Light a torch
   - Reveals a 7 x 7 area around the character
 - `s`: Saves the game
   - See the `Saving Games` section for notes
 - `b`: Turn left
 - `n`: Turn right
 - `m`: Move forward

#### Spells

The initial number of each spells is the character's Aura at multiplied by four if the spell requires the Necronomicon or by three if the spell requires the Scrolls. Casting a spell reduces the character's Aura by 0.2.

When casting a spell, the player is prompted to type a spell number. The spell numbers are:
 - From the Necronomicon:
    - `1`: Superzap
      - Kills the active monster
    - `2`: Sanctuary
      - Converts the current location to a Safe Place if empty
    - `3`: Teleport
      - Teleports the character to a random location
 - From the Scrolls:
    - `4`: Powersurge
      - Increases the character's Strength and Vitality. The increase in the number of points is between one and the number of remaining Powersurges
      - Reduces the character's Aura by an additional point
    - `5`: Metamorphosis
      - Changes the current location into a random item.
    - `6`: Healing
      - Returns the character's Strength and Vitality to their initial values.
      - Reduces the character's Aura by an additional point

#### Saving Games

The game saves by writing a new copy of `LEVEL` and `HERO`. Since the file format for `HERO` does not differentiate between current and initial attribute values, the current values become the new initial values upon reload. The file format also does not track spells remaining, so they are reset upon reload based upon the character's new initial Aura. Issue #19 exists to clean this up.
