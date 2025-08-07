# Dragon's Labyrinth - System Patterns

## System Architecture

### Modular Design
The game is organized into distinct modules, each handling specific responsibilities:

```
game.h       → Central header with all structures and declarations
main.c       → Game loop, initialization, event handling
sprites.c    → Procedural sprite generation
maze.c       → Dungeon generation and entity placement
combat.c     → Turn-based combat mechanics
ui.c         → User interface and rendering
audio.c      → Synthesized music and sound effects
```

## Key Technical Decisions

### 1. Procedural Content Generation
- **Sprites**: All pixel art generated algorithmically at runtime
- **Maze**: Recursive backtracking algorithm for dungeon layout
- **Audio**: Synthesized music using sine waves and harmonics
- **Benefits**: Zero external dependencies, smaller executable

### 2. State Management
- **Game States**: TITLE, PLAYING, COMBAT, GAME_OVER, VICTORY
- **Combat States**: Player turn → Monster turn → Resolution
- **Event-driven**: SDL event loop drives all state transitions

### 3. Rendering Architecture
- **Surface-based**: Uses SDL_Surface for compatibility
- **Double buffering**: Screen surface with blitting
- **Layered rendering**: Background → Entities → UI → Overlays

## Design Patterns in Use

### 1. Entity Component Pattern
```c
typedef struct {
    int x, y;           // Position component
    int hp, max_hp;     // Health component
    int attack, defense; // Combat component
    EntityType type;    // Type identifier
} Entity;
```

### 2. Game Loop Pattern
```
while (running) {
    Handle Events
    Update Game State
    Render Frame
    Control Frame Rate
}
```

### 3. Factory Pattern (Sprite Generation)
- `create_knight_sprite()`, `create_goblin_sprite()`, etc.
- Each function encapsulates sprite creation logic

## Component Relationships

### Core Dependencies
```
main.c
  ├─→ game.h (structures, constants)
  ├─→ sprites.c (visual assets)
  ├─→ maze.c (level generation)
  ├─→ combat.c (battle system)
  ├─→ ui.c (interface rendering)
  └─→ audio.c (sound system)
```

### Data Flow
1. **Input**: SDL Events → Event Handler → Game State Update
2. **Logic**: Game State → System Updates → Entity Updates
3. **Output**: Render State → SDL Surface → Screen

## Critical Implementation Paths

### 1. Maze Generation
- Start with filled grid
- Recursive backtracking carves paths
- Strategic entity placement after generation
- Ensures solvable paths to objectives

### 2. Combat Resolution
- Damage = (Attacker.attack - Defender.defense) + random(1,5)
- Minimum damage of 1
- Defense reduces damage by 50% when defending
- Can't run from dragon (boss mechanic)

### 3. Sprite Rendering
- 16x16 pixel art generated procedurally
- Color palettes define entity appearance
- Pixel-by-pixel construction for detail
- Cached after initial generation

### 4. Audio Synthesis
- Background music: 3-voice harmony
- Sound effects: Frequency modulation
- All generated using SDL audio callbacks
- No external audio files required
