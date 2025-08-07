# Makefile for Knight's Quest - MacOS
CC = clang
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
TARGET = knights_quest

# SDL2 paths for macOS (using Homebrew installation)
SDL_CFLAGS = `sdl2-config --cflags`
SDL_LIBS = `sdl2-config --libs` -lSDL2_mixer

# Source files
SOURCES = main.c sprites.c maze.c combat.c ui.c audio.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = game.h

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(SDL_LIBS) -lm

# Compile source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install dependencies on macOS (requires Homebrew)
install-deps:
	@echo "Installing SDL2 dependencies via Homebrew..."
	brew install sdl2 sdl2_mixer

# Run the game
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Release build
release: CFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

# Check if SDL2 is properly installed
check-sdl:
	@echo "Checking SDL2 installation..."
	@sdl2-config --version || (echo "SDL2 not found. Run 'make install-deps' first." && exit 1)
	@echo "SDL2 is properly installed."

.PHONY: all clean install-deps run debug release check-sdl

# Help target
help:
	@echo "Knight's Quest Makefile"
	@echo "======================="
	@echo "Available targets:"
	@echo "  all          - Build the game (default)"
	@echo "  clean        - Remove build files"
	@echo "  install-deps - Install SDL2 dependencies via Homebrew"
	@echo "  run          - Build and run the game"
	@echo "  debug        - Build debug version"
	@echo "  release      - Build optimized release version"
	@echo "  check-sdl    - Check SDL2 installation"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Prerequisites:"
	@echo "  - Xcode Command Line Tools: xcode-select --install"
	@echo "  - Homebrew: /bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
	@echo "  - SDL2 libraries: make install-deps"