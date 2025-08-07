#include "game.h"

int main(int argc, char* argv[]) {
    Game game = {0};
    
    if (init_game(&game) != 0) {
        fprintf(stderr, "Failed to initialize game\n");
        return 1;
    }
    
    bool quit = false;
    SDL_Event e;
    
    Uint32 frame_start, frame_time;
    const int FPS = 60;
    const int frame_delay = 1000 / FPS;
    
    while (!quit) {
        frame_start = SDL_GetTicks();
        
        while (SDL_PollEvent(&e)) {
            handle_events(&game, &e, &quit);
        }
        
        update_game(&game);
        render_game(&game);
        
        frame_time = SDL_GetTicks() - frame_start;
        if (frame_delay > frame_time) {
            SDL_Delay(frame_delay - frame_time);
        }
        
        game.frame_count++;
    }
    
    cleanup_game(&game);
    return 0;
}

int init_game(Game* game) {
    srand(time(NULL));
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    
    game->window = SDL_CreateWindow("Knight's Quest", 
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (game->window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    
    game->screen_surface = SDL_GetWindowSurface(game->window);
    
    if (init_audio(game) != 0) {
        fprintf(stderr, "Failed to initialize audio\n");
        return -1;
    }
    
    game->state = GAME_STATE_TITLE;
    
    // Initialize player
    game->world.player.x = 1;
    game->world.player.y = 1;
    game->world.player.hp = game->world.player.max_hp = 100;
    game->world.player.attack = 20;
    game->world.player.defense = 10;
    game->world.player.level = 1;
    game->world.player.xp = 0;
    game->world.player.xp_to_next = 100;
    game->world.player.has_treasure = false;
    game->world.player.sprite = create_knight_sprite();
    
    // Generate initial maze
    generate_maze(game->world.maze);
    place_entities(&game->world);
    
    return 0;
}

void cleanup_game(Game* game) {
    if (game->world.player.sprite) SDL_FreeSurface(game->world.player.sprite);
    
    for (int i = 0; i < game->world.monster_count; i++) {
        if (game->world.monsters[i].sprite) SDL_FreeSurface(game->world.monsters[i].sprite);
    }
    
    if (game->bg_music) Mix_FreeMusic(game->bg_music);
    if (game->combat_sound) Mix_FreeChunk(game->combat_sound);
    if (game->pickup_sound) Mix_FreeChunk(game->pickup_sound);
    
    Mix_CloseAudio();
    
    if (game->window) SDL_DestroyWindow(game->window);
    SDL_Quit();
}

void handle_events(Game* game, SDL_Event* e, bool* quit) {
    if (e->type == SDL_QUIT) {
        *quit = true;
        return;
    }
    
    switch (game->state) {
        case GAME_STATE_TITLE:
            if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE) {
                game->state = GAME_STATE_PLAYING;
                Mix_PlayMusic(game->bg_music, -1);
            }
            break;
            
        case GAME_STATE_PLAYING:
            if (e->type == SDL_KEYDOWN) {
                int new_x = game->world.player.x;
                int new_y = game->world.player.y;
                
                switch (e->key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_w:
                        new_y--;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        new_y++;
                        break;
                    case SDLK_LEFT:
                    case SDLK_a:
                        new_x--;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        new_x++;
                        break;
                }
                
                // Check bounds and walls
                if (new_x >= 0 && new_x < MAZE_WIDTH && new_y >= 0 && new_y < MAZE_HEIGHT) {
                    if (game->world.maze[new_y][new_x] != TILE_WALL) {
                        // Check for monsters
                        bool monster_found = false;
                        for (int i = 0; i < game->world.monster_count; i++) {
                            if (game->world.monsters[i].alive && 
                                game->world.monsters[i].x == new_x && 
                                game->world.monsters[i].y == new_y) {
                                start_combat(game, &game->world.monsters[i]);
                                monster_found = true;
                                break;
                            }
                        }
                        
                        if (!monster_found) {
                            game->world.player.x = new_x;
                            game->world.player.y = new_y;
                            
                            // Check for treasure
                            if (game->world.maze[new_y][new_x] == TILE_TREASURE) {
                                game->world.player.has_treasure = true;
                                game->world.treasure_found = true;
                                game->world.maze[new_y][new_x] = TILE_FLOOR;
                                play_pickup_sound(game);
                            }
                            
                            // Check for dragon
                            if (game->world.maze[new_y][new_x] == TILE_DRAGON) {
                                // Find dragon monster
                                for (int i = 0; i < game->world.monster_count; i++) {
                                    if (game->world.monsters[i].type == 3 && game->world.monsters[i].alive) {
                                        start_combat(game, &game->world.monsters[i]);
                                        break;
                                    }
                                }
                            }
                            
                            // Check for potions
                            for (int i = 0; i < game->world.potion_count; i++) {
                                if (!game->world.potions[i].collected &&
                                    game->world.potions[i].x == new_x &&
                                    game->world.potions[i].y == new_y) {
                                    game->world.potions[i].collected = true;
                                    game->world.player.hp = game->world.player.max_hp;
                                    play_pickup_sound(game);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            break;
            
        case GAME_STATE_COMBAT:
            handle_combat(game, e);
            break;
            
        case GAME_STATE_VICTORY:
        case GAME_STATE_GAME_OVER:
            if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE) {
                // Restart game
                game->state = GAME_STATE_TITLE;
                game->world.player.x = 1;
                game->world.player.y = 1;
                game->world.player.hp = game->world.player.max_hp = 100;
                game->world.player.level = 1;
                game->world.player.xp = 0;
                game->world.player.has_treasure = false;
                game->world.treasure_found = false;
                game->world.dragon_defeated = false;
                generate_maze(game->world.maze);
                place_entities(&game->world);
                Mix_HaltMusic();
            }
            break;
    }
}

void update_game(Game* game) {
    switch (game->state) {
        case GAME_STATE_PLAYING:
            // Check victory condition
            if (game->world.treasure_found && game->world.dragon_defeated) {
                game->state = GAME_STATE_VICTORY;
                Mix_HaltMusic();
            }
            
            // Check game over condition
            if (game->world.player.hp <= 0) {
                game->state = GAME_STATE_GAME_OVER;
                Mix_HaltMusic();
            }
            break;
        default:
            break;
    }
}

void render_game(Game* game) {
    // Clear screen
    SDL_FillRect(game->screen_surface, NULL, SDL_MapRGB(game->screen_surface->format, 0, 0, 0));
    
    switch (game->state) {
        case GAME_STATE_TITLE:
            render_title_screen(game);
            break;
            
        case GAME_STATE_PLAYING:
            // Render maze
            for (int y = 0; y < MAZE_HEIGHT; y++) {
                for (int x = 0; x < MAZE_WIDTH; x++) {
                    SDL_Rect tile_rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    Uint32 color;
                    
                    switch (game->world.maze[y][x]) {
                        case TILE_WALL:
                            color = SDL_MapRGB(game->screen_surface->format, 100, 100, 100);
                            break;
                        case TILE_FLOOR:
                            color = SDL_MapRGB(game->screen_surface->format, 200, 200, 200);
                            break;
                        case TILE_TREASURE:
                            color = SDL_MapRGB(game->screen_surface->format, 255, 215, 0);
                            break;
                        case TILE_DRAGON:
                            color = SDL_MapRGB(game->screen_surface->format, 139, 0, 0);
                            break;
                        default:
                            color = SDL_MapRGB(game->screen_surface->format, 200, 200, 200);
                            break;
                    }
                    
                    SDL_FillRect(game->screen_surface, &tile_rect, color);
                }
            }
            
            // Render potions
            for (int i = 0; i < game->world.potion_count; i++) {
                if (!game->world.potions[i].collected) {
                    SDL_Surface* potion_sprite = create_potion_sprite();
                    SDL_Rect potion_rect = {
                        game->world.potions[i].x * TILE_SIZE,
                        game->world.potions[i].y * TILE_SIZE,
                        TILE_SIZE, TILE_SIZE
                    };
                    SDL_BlitScaled(potion_sprite, NULL, game->screen_surface, &potion_rect);
                    SDL_FreeSurface(potion_sprite);
                }
            }
            
            // Render monsters
            for (int i = 0; i < game->world.monster_count; i++) {
                if (game->world.monsters[i].alive) {
                    SDL_Rect monster_rect = {
                        game->world.monsters[i].x * TILE_SIZE,
                        game->world.monsters[i].y * TILE_SIZE,
                        TILE_SIZE, TILE_SIZE
                    };
                    SDL_BlitScaled(game->world.monsters[i].sprite, NULL, game->screen_surface, &monster_rect);
                }
            }
            
            // Render player
            SDL_Rect player_rect = {
                game->world.player.x * TILE_SIZE,
                game->world.player.y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE
            };
            SDL_BlitScaled(game->world.player.sprite, NULL, game->screen_surface, &player_rect);
            
            render_ui(game);
            break;
            
        case GAME_STATE_COMBAT:
            render_combat(game);
            break;
            
        case GAME_STATE_VICTORY:
            render_title_screen(game);
            // Add victory text overlay
            break;
            
        case GAME_STATE_GAME_OVER:
            render_title_screen(game);
            // Add game over text overlay
            break;
    }
    
    SDL_UpdateWindowSurface(game->window);
}