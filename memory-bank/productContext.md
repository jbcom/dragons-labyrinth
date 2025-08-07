# Dragon's Labyrinth - Product Context

## Why This Project Exists
Dragon's Labyrinth was created to demonstrate a complete, self-contained dungeon crawler game that requires no external assets. It showcases procedural content generation and modular game architecture.

## Problems It Solves
1. **Complete Game Experience**: Provides a full game loop from title screen to victory condition
2. **No Asset Dependencies**: All sprites and audio generated programmatically
3. **Educational Example**: Demonstrates clean SDL2 game architecture
4. **Cross-platform Gaming**: Optimized for macOS but portable design

## How It Should Work

### Game Flow
1. **Title Screen**: Display game title with knight/wizard sprites, wait for SPACE to start
2. **Dungeon Generation**: Create procedural maze with strategic entity placement
3. **Exploration Phase**: Navigate maze, encounter monsters, collect items
4. **Combat Phase**: Turn-based battles with attack/defend/run options
5. **Victory Condition**: Find treasure AND defeat dragon boss

### User Experience Goals
- **Intuitive Controls**: WASD/Arrow keys for movement, simple combat controls
- **Visual Clarity**: Clear HP/XP bars, minimap for navigation
- **Audio Feedback**: Synthesized music and sound effects enhance immersion
- **Progressive Difficulty**: Monsters scale from goblins to dragon boss
- **Rewarding Progression**: Level ups provide meaningful stat increases

## Core Gameplay Loop
1. Explore maze → Find monsters/items
2. Engage combat → Gain XP/potions
3. Level up → Become stronger
4. Find treasure → Unlock dragon fight
5. Defeat dragon → Achieve victory

## Unique Value Proposition
- Completely self-contained game (no external files needed)
- Procedurally generated content ensures replayability
- Classic dungeon crawler mechanics in a modern implementation
- Educational codebase with clean architecture
