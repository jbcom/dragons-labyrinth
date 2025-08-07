# Knight's Quest - A Dungeon Crawler Adventure

A complete SDL2-based dungeon crawler game featuring a brave knight on a quest to find treasure and defeat a mighty dragon in an ever-expanding maze-generated dungeon.

## Features

### Gameplay
- **Procedurally Generated Dungeons**: Each playthrough features a unique maze layout generated using recursive backtracking algorithm
- **RPG Elements**: Level up system with XP, HP, attack, and defense progression
- **Combat System**: Turn-based combat with attack, defend, and run options
- **Monster Variety**: Fight goblins, orcs, skeletons, and face the mighty dragon boss
- **Quest System**: Find the treasure and defeat the dragon to win
- **Potion System**: Collect healing potions as random drops from defeated monsters
- **Real-time UI**: Health bars, XP progression, minimap, and quest tracking

### Technical Features
- **16-bit Pixel Art**: All sprites generated procedurally with authentic retro aesthetic
- **Synthesized Audio**: Dynamic background music and sound effects created programmatically
- **Efficient Architecture**: Modular code structure with separate systems for graphics, audio, combat, and maze generation
- **MacOS Optimized**: Native compilation and optimization for Apple Silicon and Intel Macs

## Controls

### Title Screen
- **SPACE**: Start new game

### Gameplay
- **WASD** or **Arrow Keys**: Move knight
- **Collision Detection**: Automatic combat initiation when encountering monsters

### Combat
- **SPACE** or **A**: Attack enemy
- **D**: Defend (reduces incoming damage by 50%)
- **R**: Run away (not available against dragon)

### End Game
- **SPACE**: Restart game after victory or game over

## Installation & Compilation

### Prerequisites

1. **Xcode Command Line Tools**:
   ```bash
   xcode-select --install
   ```

2. **Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **SDL2 Libraries**:
   ```bash
   make install-deps
   ```

### Building the Game

```bash
# Quick build and run
make run

# Or build separately
make all
./knights_quest

# Debug build
make debug

# Optimized release build
make release
```

### Verification

Check if SDL2 is properly installed:
```bash
make check-sdl
```

## Game Systems

### Maze Generation
- Uses recursive backtracking algorithm for natural-looking dungeon layouts
- Additional pathways added for improved gameplay flow
- Guaranteed solvable paths between all accessible areas

### Combat System
- Turn-based with damage calculation: `damage = attack - defense + random_modifier`
- Experience points awarded based on enemy max HP
- Level progression increases all stats and fully restores health
- Monster difficulty scales with type (goblin < orc < skeleton < dragon)

### Monster AI
- **Goblins**: Weak but numerous (HP: 30, Attack: 10, Defense: 5)
- **Orcs**: Balanced fighters (HP: 50, Attack: 15, Defense: 8)  
- **Skeletons**: Fragile but dangerous (HP: 70, Attack: 20, Defense: 11)
- **Dragon**: Boss enemy (HP: 200, Attack: 50, Defense: 30)

### Progression System
- Level up increases: +20 HP, +3 Attack, +2 Defense
- XP requirement increases by 50 per level
- Full health restoration on level up

## Code Architecture

```
├── game.h          # Main header with all declarations
├── main.c          # Game loop and initialization  
├── sprites.c       # Procedural pixel art generation
├── maze.c          # Dungeon generation and entity placement
├── combat.c        # Turn-based combat system
├── ui.c            # User interface rendering
├── audio.c         # Synthesized sound effects
├── Makefile        # Build configuration
└── README.md       # This file
```

## Audio System

All audio is generated programmatically:

- **Background Music**: Procedural melodies using C major scale with harmony and bass lines
- **Combat Sounds**: Synthesized sword clashes with metallic resonance and decay
- **Pickup Sounds**: Pleasant ascending chimes for item collection

## Graphics System

All sprites are generated as 16x16 pixel art:

- **Knight**: Armored warrior with sword and shield
- **Wizard**: Robed figure with staff and magical orb (appears in title screen)
- **Monsters**: Unique designs for each enemy type with appropriate color schemes
- **Items**: Treasure chests and healing potions with distinctive visual elements

## Performance Optimizations

- **Efficient Rendering**: Surface-based graphics with minimal memory allocations
- **Smart UI Updates**: Only essential interface elements updated per frame  
- **Optimized Audio**: Short sound clips with efficient synthesis
- **Memory Management**: Proper cleanup of all SDL resources

## Troubleshooting

### Common Issues

1. **SDL2 not found**: Run `make install-deps` to install required libraries
2. **Compilation errors**: Ensure Xcode Command Line Tools are installed
3. **Audio issues**: Check that SDL2_mixer is properly installed
4. **Performance issues**: Try the release build with `make release`

### Build Targets

```bash
make help          # Show all available commands
make clean         # Remove all build files
make debug         # Build with debug symbols
make release       # Build optimized version
make check-sdl     # Verify SDL2 installation
```

## Game Tips

1. **Combat Strategy**: Use defend against strong enemies to reduce damage
2. **Exploration**: Check the minimap to track your progress and find unexplored areas
3. **Resource Management**: Save potions for tough battles, especially the dragon fight
4. **Leveling**: Fight all monsters to gain XP before facing the dragon
5. **Quest Order**: Find treasure first, then locate and defeat the dragon for victory

## Future Enhancements

Potential additions for expanded versions:
- Multiple dungeon levels with increasing difficulty
- Equipment system with weapons and armor
- Spell casting for the player character
- Save/load game functionality
- Achievement system
- Multiplayer co-op mode

## License

This project is released under the MIT License. Feel free to modify and distribute according to the license terms.

## Credits

- **Game Design & Programming**: Generated procedurally with careful attention to classic dungeon crawler mechanics
- **Audio Synthesis**: Mathematical wave generation for authentic chiptune-style audio
- **Pixel Art**: Programmatically created sprites inspired by 16-bit era aesthetics