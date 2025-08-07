#!/bin/bash

# Knight's Quest - Automated Setup Script for macOS
# This script will install dependencies and build the game

set -e  # Exit on any error

echo "🏰 Knight's Quest - Automated Setup"
echo "==================================="
echo

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    print_error "This script is designed for macOS only."
    exit 1
fi

print_status "Checking system requirements..."

# Check for Xcode Command Line Tools
if ! xcode-select -p &> /dev/null; then
    print_warning "Xcode Command Line Tools not found."
    print_status "Installing Xcode Command Line Tools..."
    xcode-select --install
    echo "Please complete the Xcode Command Line Tools installation and run this script again."
    exit 1
else
    print_success "Xcode Command Line Tools found."
fi

# Check for Homebrew
if ! command -v brew &> /dev/null; then
    print_warning "Homebrew not found. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    
    # Add Homebrew to PATH for this session
    if [[ -f "/opt/homebrew/bin/brew" ]]; then
        export PATH="/opt/homebrew/bin:$PATH"
    elif [[ -f "/usr/local/bin/brew" ]]; then
        export PATH="/usr/local/bin:$PATH"
    fi
    
    print_success "Homebrew installed successfully."
else
    print_success "Homebrew found."
fi

# Update Homebrew
print_status "Updating Homebrew..."
brew update

# Install SDL2 dependencies
print_status "Installing SDL2 dependencies..."

if brew list sdl2 &>/dev/null; then
    print_success "SDL2 already installed."
else
    print_status "Installing SDL2..."
    brew install sdl2
    print_success "SDL2 installed."
fi

if brew list sdl2_mixer &>/dev/null; then
    print_success "SDL2_mixer already installed."
else
    print_status "Installing SDL2_mixer..."
    brew install sdl2_mixer
    print_success "SDL2_mixer installed."
fi

# Verify SDL2 installation
print_status "Verifying SDL2 installation..."
if sdl2-config --version &>/dev/null; then
    SDL_VERSION=$(sdl2-config --version)
    print_success "SDL2 version $SDL_VERSION is properly installed."
else
    print_error "SDL2 installation verification failed."
    exit 1
fi

# Build the game
print_status "Building Knight's Quest..."
if make clean && make all; then
    print_success "Game built successfully!"
else
    print_error "Build failed. Please check the error messages above."
    exit 1
fi

# Check if executable was created
if [[ -f "./knights_quest" ]]; then
    print_success "Game executable created: ./knights_quest"
else
    print_error "Game executable not found after build."
    exit 1
fi

echo
echo "🎉 Setup Complete!"
echo "=================="
print_success "Knight's Quest is ready to play!"
echo
echo "To start the game:"
echo "  ./knights_quest"
echo "or"
echo "  make run"
echo
echo "Game Controls:"
echo "  Title Screen: SPACE to start"
echo "  Movement: WASD or Arrow Keys"
echo "  Combat: SPACE/A to attack, D to defend, R to run"
echo
echo "For more information, see README.md"
echo

# Ask if user wants to run the game now
read -p "Would you like to start the game now? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    print_status "Starting Knight's Quest..."
    ./knights_quest
fi