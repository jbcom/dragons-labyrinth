# Dragon's Labyrinth - Technical Context

## Technologies Used

### Core Dependencies
- **SDL2**: Simple DirectMedia Layer 2.x
  - Cross-platform multimedia library
  - Handles graphics, audio, and input
  - Version: Latest stable (installed via Homebrew)

### Development Tools
- **Compiler**: Clang (Xcode Command Line Tools)
- **Build System**: GNU Make
- **Package Manager**: Homebrew (for SDL2 installation)
- **Platform**: macOS (primary target)

## Development Setup

### Prerequisites
1. **Xcode Command Line Tools**
   ```bash
   xcode-select --install
   ```

2. **Homebrew**
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **SDL2 Library**
   ```bash
   brew install sdl2
   ```

### Build Configuration
- **Debug Mode**: `-g` flag for debugging symbols
- **Release Mode**: `-O2` optimization
- **Warnings**: `-Wall -Wextra` for comprehensive error checking
- **C Standard**: C11 (`-std=c11`)

## Technical Constraints

### Memory Management
- Manual memory allocation/deallocation
- Surface cleanup on exit
- Audio buffer management
- No memory leaks in game loop

### Performance Considerations
- 60 FPS target (16ms per frame)
- Efficient sprite caching
- Optimized maze generation
- Minimal SDL surface operations

### Platform-Specific
- macOS framework linking
- Homebrew library paths
- Universal binary support

## Dependencies Details

### SDL2 Components Used
- **Core**: Window management, event handling
- **Surface**: 2D graphics rendering
- **Audio**: Sound synthesis and playback
- **Timer**: Frame rate control

### No External Assets
- All graphics generated programmatically
- All audio synthesized in real-time
- No image/sound file dependencies
- Self-contained executable

## Tool Usage Patterns

### Makefile Targets
- `make all`: Standard build
- `make debug`: Debug build with symbols
- `make release`: Optimized release build
- `make run`: Build and execute
- `make clean`: Remove build artifacts
- `make install-deps`: Install SDL2 via Homebrew

### Compilation Flags
```makefile
CFLAGS = -Wall -Wextra -std=c11 $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
```

### Directory Structure
```
dragons-labyrinth/
├── game.h        # Central header
├── main.c        # Entry point
├── sprites.c     # Graphics generation
├── maze.c        # Level generation
├── combat.c      # Battle system
├── ui.c          # Interface
├── audio.c       # Sound system
├── Makefile      # Build configuration
├── README.md     # Documentation
├── setup.sh      # Automated setup
└── memory-bank/  # Project memory
```

## Known Technical Decisions

### Why SDL_Surface over SDL_Renderer
- Simpler API for 2D pixel art
- Direct pixel manipulation
- Better for procedural generation
- Sufficient performance for game scope

### Why Procedural Generation
- Zero external dependencies
- Smaller executable size
- Demonstrates algorithmic thinking
- Educational value

### Audio Implementation
- SDL audio callbacks for synthesis
- Simple waveform generation
- No external audio libraries
- Real-time sound generation
