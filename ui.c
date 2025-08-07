#include "game.h"

void render_ui(Game* game) {
    // HP display in top left
    SDL_Rect hp_bg = {10, 10, 200, 30};
    SDL_FillRect(game->screen_surface, &hp_bg, SDL_MapRGB(game->screen_surface->format, 50, 50, 50));
    
    SDL_Rect hp_border = {8, 8, 204, 34};
    SDL_FillRect(game->screen_surface, &hp_border, SDL_MapRGB(game->screen_surface->format, 255, 255, 255));
    
    int hp_width = (190 * game->world.player.hp) / game->world.player.max_hp;
    if (hp_width > 0) {
        SDL_Rect hp_bar = {15, 15, hp_width, 20};
        Uint32 hp_color;
        if (game->world.player.hp > game->world.player.max_hp * 0.6) {
            hp_color = SDL_MapRGB(game->screen_surface->format, 0, 255, 0);
        } else if (game->world.player.hp > game->world.player.max_hp * 0.3) {
            hp_color = SDL_MapRGB(game->screen_surface->format, 255, 255, 0);
        } else {
            hp_color = SDL_MapRGB(game->screen_surface->format, 255, 0, 0);
        }
        SDL_FillRect(game->screen_surface, &hp_bar, hp_color);
    }
    
    // HP text representation (simple blocks for digits)
    char hp_text[32];
    snprintf(hp_text, sizeof(hp_text), "HP: %d/%d", game->world.player.hp, game->world.player.max_hp);
    
    // Level display
    SDL_Rect level_bg = {SCREEN_WIDTH - 150, 10, 140, 30};
    SDL_FillRect(game->screen_surface, &level_bg, SDL_MapRGB(game->screen_surface->format, 50, 50, 100));
    
    SDL_Rect level_border = {SCREEN_WIDTH - 152, 8, 144, 34};
    SDL_FillRect(game->screen_surface, &level_border, SDL_MapRGB(game->screen_surface->format, 255, 255, 255));
    
    // XP bar at the bottom
    SDL_Rect xp_bg = {50, SCREEN_HEIGHT - 40, SCREEN_WIDTH - 100, 20};
    SDL_FillRect(game->screen_surface, &xp_bg, SDL_MapRGB(game->screen_surface->format, 50, 50, 50));
    
    SDL_Rect xp_border = {48, SCREEN_HEIGHT - 42, SCREEN_WIDTH - 96, 24};
    SDL_FillRect(game->screen_surface, &xp_border, SDL_MapRGB(game->screen_surface->format, 255, 255, 255));
    
    int xp_width = ((SCREEN_WIDTH - 110) * game->world.player.xp) / game->world.player.xp_to_next;
    if (xp_width > 0) {
        SDL_Rect xp_bar = {55, SCREEN_HEIGHT - 35, xp_width, 10};
        SDL_FillRect(game->screen_surface, &xp_bar, SDL_MapRGB(game->screen_surface->format, 0, 0, 255));
    }
    
    // Status indicators
    if (game->world.player.has_treasure) {
        SDL_Rect treasure_indicator = {220, 10, 30, 30};
        SDL_FillRect(game->screen_surface, &treasure_indicator, SDL_MapRGB(game->screen_surface->format, 255, 215, 0));
        
        // Add some detail to make it look like a treasure icon
        SDL_Rect treasure_detail = {225, 15, 20, 20};
        SDL_FillRect(game->screen_surface, &treasure_detail, SDL_MapRGB(game->screen_surface->format, 218, 165, 32));
    }
    
    // Quest status
    SDL_Rect quest_bg = {10, 50, 300, 80};
    SDL_FillRect(game->screen_surface, &quest_bg, SDL_MapRGB(game->screen_surface->format, 40, 40, 40));
    
    SDL_Rect quest_border = {8, 48, 304, 84};
    SDL_FillRect(game->screen_surface, &quest_border, SDL_MapRGB(game->screen_surface->format, 200, 200, 200));
    
    // Quest objectives (represented as colored bars)
    SDL_Rect objective1 = {15, 60, 280, 15};
    Uint32 obj1_color = game->world.treasure_found ? 
        SDL_MapRGB(game->screen_surface->format, 0, 200, 0) :
        SDL_MapRGB(game->screen_surface->format, 200, 0, 0);
    SDL_FillRect(game->screen_surface, &objective1, obj1_color);
    
    SDL_Rect objective2 = {15, 80, 280, 15};
    Uint32 obj2_color = game->world.dragon_defeated ? 
        SDL_MapRGB(game->screen_surface->format, 0, 200, 0) :
        SDL_MapRGB(game->screen_surface->format, 200, 0, 0);
    SDL_FillRect(game->screen_surface, &objective2, obj2_color);
    
    // Mini-map in top right
    SDL_Rect minimap_bg = {SCREEN_WIDTH - 160, 50, 150, 120};
    SDL_FillRect(game->screen_surface, &minimap_bg, SDL_MapRGB(game->screen_surface->format, 20, 20, 20));
    
    SDL_Rect minimap_border = {SCREEN_WIDTH - 162, 48, 154, 124};
    SDL_FillRect(game->screen_surface, &minimap_border, SDL_MapRGB(game->screen_surface->format, 150, 150, 150));
    
    // Draw minimap
    int map_scale = 5;
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            SDL_Rect mini_tile = {
                SCREEN_WIDTH - 155 + x * map_scale,
                55 + y * map_scale,
                map_scale - 1, map_scale - 1
            };
            
            Uint32 color;
            switch (game->world.maze[y][x]) {
                case TILE_WALL:
                    color = SDL_MapRGB(game->screen_surface->format, 80, 80, 80);
                    break;
                case TILE_FLOOR:
                    color = SDL_MapRGB(game->screen_surface->format, 180, 180, 180);
                    break;
                case TILE_TREASURE:
                    color = SDL_MapRGB(game->screen_surface->format, 255, 215, 0);
                    break;
                case TILE_DRAGON:
                    color = SDL_MapRGB(game->screen_surface->format, 255, 0, 0);
                    break;
                default:
                    color = SDL_MapRGB(game->screen_surface->format, 180, 180, 180);
                    break;
            }
            
            SDL_FillRect(game->screen_surface, &mini_tile, color);
        }
    }
    
    // Player position on minimap
    SDL_Rect player_dot = {
        SCREEN_WIDTH - 155 + game->world.player.x * map_scale,
        55 + game->world.player.y * map_scale,
        map_scale - 1, map_scale - 1
    };
    SDL_FillRect(game->screen_surface, &player_dot, SDL_MapRGB(game->screen_surface->format, 0, 255, 0));
    
    // Monster positions on minimap
    for (int i = 0; i < game->world.monster_count; i++) {
        if (game->world.monsters[i].alive) {
            SDL_Rect monster_dot = {
                SCREEN_WIDTH - 155 + game->world.monsters[i].x * map_scale,
                55 + game->world.monsters[i].y * map_scale,
                map_scale - 1, map_scale - 1
            };
            Uint32 monster_color = (game->world.monsters[i].type == 3) ?
                SDL_MapRGB(game->screen_surface->format, 200, 0, 200) :
                SDL_MapRGB(game->screen_surface->format, 255, 100, 100);
            SDL_FillRect(game->screen_surface, &monster_dot, monster_color);
        }
    }
}

void render_title_screen(Game* game) {
    // Background
    SDL_FillRect(game->screen_surface, NULL, SDL_MapRGB(game->screen_surface->format, 20, 20, 60));
    
    // Title background
    SDL_Rect title_bg = {SCREEN_WIDTH/2 - 200, 100, 400, 80};
    SDL_FillRect(game->screen_surface, &title_bg, SDL_MapRGB(game->screen_surface->format, 100, 100, 150));
    
    SDL_Rect title_border = {SCREEN_WIDTH/2 - 205, 95, 410, 90};
    SDL_FillRect(game->screen_surface, &title_border, SDL_MapRGB(game->screen_surface->format, 255, 255, 255));
    
    // Decorative elements around title
    for (int i = 0; i < 20; i++) {
        SDL_Rect star = {
            SCREEN_WIDTH/2 - 300 + (rand() % 600),
            50 + (rand() % 100),
            3, 3
        };
        SDL_FillRect(game->screen_surface, &star, SDL_MapRGB(game->screen_surface->format, 255, 255, 100));
    }
    
    // Knight sprite (large)
    SDL_Surface* knight_sprite = create_knight_sprite();
    if (knight_sprite) {
        SDL_Rect knight_rect = {SCREEN_WIDTH/2 - 150, 250, 100, 100};
        SDL_BlitScaled(knight_sprite, NULL, game->screen_surface, &knight_rect);
        SDL_FreeSurface(knight_sprite);
    }
    
    // Wizard sprite (large)
    SDL_Surface* wizard_sprite = create_wizard_sprite();
    if (wizard_sprite) {
        SDL_Rect wizard_rect = {SCREEN_WIDTH/2 + 50, 250, 100, 100};
        SDL_BlitScaled(wizard_sprite, NULL, game->screen_surface, &wizard_rect);
        SDL_FreeSurface(wizard_sprite);
    }
    
    // Instructions background
    SDL_Rect instructions_bg = {SCREEN_WIDTH/2 - 250, 400, 500, 120};
    SDL_FillRect(game->screen_surface, &instructions_bg, SDL_MapRGB(game->screen_surface->format, 60, 60, 60));
    
    SDL_Rect instructions_border = {SCREEN_WIDTH/2 - 255, 395, 510, 130};
    SDL_FillRect(game->screen_surface, &instructions_border, SDL_MapRGB(game->screen_surface->format, 200, 200, 200));
    
    // Simple instruction indicators
    SDL_Rect space_key = {SCREEN_WIDTH/2 - 200, 420, 150, 30};
    SDL_FillRect(game->screen_surface, &space_key, SDL_MapRGB(game->screen_surface->format, 100, 200, 100));
    
    SDL_Rect wasd_keys = {SCREEN_WIDTH/2 - 200, 460, 150, 30};
    SDL_FillRect(game->screen_surface, &wasd_keys, SDL_MapRGB(game->screen_surface->format, 200, 100, 100));
    
    // Game state specific messages
    switch (game->state) {
        case GAME_STATE_TITLE:
            // "Press SPACE to Start" indicator
            if ((game->frame_count / 30) % 2) { // Blinking effect
                SDL_Rect start_msg = {SCREEN_WIDTH/2 - 100, 550, 200, 30};
                SDL_FillRect(game->screen_surface, &start_msg, SDL_MapRGB(game->screen_surface->format, 255, 255, 0));
            }
            break;
            
        case GAME_STATE_VICTORY:
            SDL_Rect victory_bg = {SCREEN_WIDTH/2 - 150, 180, 300, 60};
            SDL_FillRect(game->screen_surface, &victory_bg, SDL_MapRGB(game->screen_surface->format, 0, 255, 0));
            break;
            
        case GAME_STATE_GAME_OVER:
            SDL_Rect gameover_bg = {SCREEN_WIDTH/2 - 150, 180, 300, 60};
            SDL_FillRect(game->screen_surface, &gameover_bg, SDL_MapRGB(game->screen_surface->format, 255, 0, 0));
            break;
            
        default:
            break;
    }
}