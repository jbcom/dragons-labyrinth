#include "game.h"

void start_combat(Game* game, Monster* monster) {
    game->state = GAME_STATE_COMBAT;
    game->current_enemy = monster;
    play_combat_sound(game);
}

void handle_combat(Game* game, SDL_Event* e) {
    if (e->type != SDL_KEYDOWN) return;
    
    Monster* enemy = game->current_enemy;
    if (!enemy || !enemy->alive) {
        game->state = GAME_STATE_PLAYING;
        game->current_enemy = NULL;
        return;
    }
    
    bool player_acted = false;
    
    switch (e->key.keysym.sym) {
        case SDLK_SPACE: // Attack
        case SDLK_a: {
            // Player attacks
            int damage = game->world.player.attack - enemy->defense;
            if (damage < 1) damage = 1;
            
            // Add some randomness
            damage += (rand() % 10) - 5;
            if (damage < 1) damage = 1;
            
            enemy->hp -= damage;
            player_acted = true;
            
            if (enemy->hp <= 0) {
                enemy->alive = false;
                
                // Award XP
                int xp_gained = enemy->max_hp / 2;
                game->world.player.xp += xp_gained;
                
                // Check for level up
                while (game->world.player.xp >= game->world.player.xp_to_next) {
                    game->world.player.xp -= game->world.player.xp_to_next;
                    game->world.player.level++;
                    game->world.player.max_hp += 20;
                    game->world.player.hp = game->world.player.max_hp;
                    game->world.player.attack += 3;
                    game->world.player.defense += 2;
                    game->world.player.xp_to_next += 50;
                }
                
                // Check if dragon was defeated
                if (enemy->type == 3) {
                    game->world.dragon_defeated = true;
                }
                
                // Random potion drop
                if (rand() % 3 == 0 && game->world.potion_count < MAX_POTIONS) {
                    game->world.potions[game->world.potion_count] = (Potion){
                        .x = enemy->x,
                        .y = enemy->y,
                        .collected = false
                    };
                    game->world.potion_count++;
                }
                
                game->state = GAME_STATE_PLAYING;
                game->current_enemy = NULL;
                return;
            }
            break;
        }
        
        case SDLK_d: // Defend (reduce incoming damage)
            player_acted = true;
            break;
            
        case SDLK_r: // Run away
            if (enemy->type != 3) { // Can't run from dragon
                game->state = GAME_STATE_PLAYING;
                game->current_enemy = NULL;
                return;
            }
            break;
    }
    
    if (player_acted) {
        // Enemy attacks back
        int damage = enemy->attack - game->world.player.defense;
        if (damage < 1) damage = 1;
        
        // If player defended, reduce damage
        if (e->key.keysym.sym == SDLK_d) {
            damage /= 2;
        }
        
        // Add some randomness
        damage += (rand() % 8) - 4;
        if (damage < 1) damage = 1;
        
        game->world.player.hp -= damage;
        
        if (game->world.player.hp <= 0) {
            game->world.player.hp = 0;
            game->state = GAME_STATE_GAME_OVER;
            game->current_enemy = NULL;
        }
    }
}

void render_combat(Game* game) {
    // Dark background
    SDL_FillRect(game->screen_surface, NULL, SDL_MapRGB(game->screen_surface->format, 20, 20, 40));
    
    Monster* enemy = game->current_enemy;
    if (!enemy) return;
    
    // Render enemy sprite (large)
    SDL_Rect enemy_rect = {SCREEN_WIDTH/2 - 64, 100, 128, 128};
    SDL_BlitScaled(enemy->sprite, NULL, game->screen_surface, &enemy_rect);
    
    // Render player sprite (large)
    SDL_Rect player_rect = {SCREEN_WIDTH/2 - 64, 300, 128, 128};
    SDL_BlitScaled(game->world.player.sprite, NULL, game->screen_surface, &player_rect);
    
    // Combat UI - simplified text rendering using colored rectangles
    
    // Enemy name background
    SDL_Rect enemy_name_bg = {SCREEN_WIDTH/2 - 100, 50, 200, 30};
    SDL_FillRect(game->screen_surface, &enemy_name_bg, SDL_MapRGB(game->screen_surface->format, 100, 0, 0));
    
    // Enemy HP bar background
    SDL_Rect enemy_hp_bg = {SCREEN_WIDTH/2 - 100, 240, 200, 20};
    SDL_FillRect(game->screen_surface, &enemy_hp_bg, SDL_MapRGB(game->screen_surface->format, 50, 50, 50));
    
    // Enemy HP bar
    int enemy_hp_width = (200 * enemy->hp) / enemy->max_hp;
    if (enemy_hp_width > 0) {
        SDL_Rect enemy_hp_bar = {SCREEN_WIDTH/2 - 100, 240, enemy_hp_width, 20};
        SDL_FillRect(game->screen_surface, &enemy_hp_bar, SDL_MapRGB(game->screen_surface->format, 200, 0, 0));
    }
    
    // Player HP bar background
    SDL_Rect player_hp_bg = {SCREEN_WIDTH/2 - 100, 450, 200, 20};
    SDL_FillRect(game->screen_surface, &player_hp_bg, SDL_MapRGB(game->screen_surface->format, 50, 50, 50));
    
    // Player HP bar
    int player_hp_width = (200 * game->world.player.hp) / game->world.player.max_hp;
    if (player_hp_width > 0) {
        SDL_Rect player_hp_bar = {SCREEN_WIDTH/2 - 100, 450, player_hp_width, 20};
        SDL_FillRect(game->screen_surface, &player_hp_bar, SDL_MapRGB(game->screen_surface->format, 0, 200, 0));
    }
    
    // Action buttons background
    SDL_Rect actions_bg = {50, 500, SCREEN_WIDTH - 100, 80};
    SDL_FillRect(game->screen_surface, &actions_bg, SDL_MapRGB(game->screen_surface->format, 60, 60, 60));
    
    // Attack button
    SDL_Rect attack_btn = {70, 520, 150, 25};
    SDL_FillRect(game->screen_surface, &attack_btn, SDL_MapRGB(game->screen_surface->format, 150, 0, 0));
    
    // Defend button
    SDL_Rect defend_btn = {240, 520, 150, 25};
    SDL_FillRect(game->screen_surface, &defend_btn, SDL_MapRGB(game->screen_surface->format, 0, 0, 150));
    
    // Run button (if not dragon)
    if (enemy->type != 3) {
        SDL_Rect run_btn = {410, 520, 150, 25};
        SDL_FillRect(game->screen_surface, &run_btn, SDL_MapRGB(game->screen_surface->format, 100, 100, 0));
    }
}