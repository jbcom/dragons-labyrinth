#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILE_SIZE 32
#define MAZE_WIDTH 25
#define MAZE_HEIGHT 18
#define MAX_MONSTERS 10
#define MAX_POTIONS 5

typedef enum {
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING,
    GAME_STATE_COMBAT,
    GAME_STATE_VICTORY,
    GAME_STATE_GAME_OVER
} GameState;

typedef enum {
    TILE_WALL,
    TILE_FLOOR,
    TILE_TREASURE,
    TILE_DRAGON,
    TILE_POTION
} TileType;

typedef struct {
    int x, y;
    int hp, max_hp;
    int attack, defense;
    int level;
    int xp;
    int xp_to_next;
    bool has_treasure;
    SDL_Surface* sprite;
} Player;

typedef struct {
    int x, y;
    int hp, max_hp;
    int attack, defense;
    int type;
    bool alive;
    SDL_Surface* sprite;
} Monster;

typedef struct {
    int x, y;
    bool collected;
} Potion;

typedef struct {
    TileType maze[MAZE_HEIGHT][MAZE_WIDTH];
    Player player;
    Monster monsters[MAX_MONSTERS];
    Potion potions[MAX_POTIONS];
    int monster_count;
    int potion_count;
    bool treasure_found;
    bool dragon_defeated;
} GameWorld;

typedef struct {
    SDL_Window* window;
    SDL_Surface* screen_surface;
    GameState state;
    GameWorld world;
    Monster* current_enemy;
    Mix_Music* bg_music;
    Mix_Chunk* combat_sound;
    Mix_Chunk* pickup_sound;
    int frame_count;
} Game;

// Function declarations
int init_game(Game* game);
void cleanup_game(Game* game);
void handle_events(Game* game, SDL_Event* e, bool* quit);
void update_game(Game* game);
void render_game(Game* game);

// Maze generation
void generate_maze(TileType maze[MAZE_HEIGHT][MAZE_WIDTH]);
void place_entities(GameWorld* world);

// Sprites
SDL_Surface* create_knight_sprite();
SDL_Surface* create_wizard_sprite();
SDL_Surface* create_monster_sprite(int type);
SDL_Surface* create_dragon_sprite();
SDL_Surface* create_treasure_sprite();
SDL_Surface* create_potion_sprite();

// Combat
void start_combat(Game* game, Monster* monster);
void handle_combat(Game* game, SDL_Event* e);
void render_combat(Game* game);

// UI
void render_ui(Game* game);
void render_title_screen(Game* game);

// Audio
int init_audio(Game* game);
void play_combat_sound(Game* game);
void play_pickup_sound(Game* game);

// Utility
SDL_Surface* create_colored_surface(int width, int height, Uint32 color);
void draw_pixel(SDL_Surface* surface, int x, int y, Uint32 color);
Uint32 get_color(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);

#endif