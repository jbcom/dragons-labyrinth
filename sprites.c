#include "game.h"

SDL_Surface* create_colored_surface(int width, int height, Uint32 color) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 
                                               0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (surface) {
        SDL_FillRect(surface, NULL, color);
    }
    return surface;
}

void draw_pixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32* pixels = (Uint32*)surface->pixels;
        pixels[y * surface->w + x] = color;
    }
}

Uint32 get_color(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b) {
    return SDL_MapRGB(surface->format, r, g, b);
}

SDL_Surface* create_knight_sprite() {
    const int size = 16;
    SDL_Surface* sprite = SDL_CreateRGBSurface(0, size, size, 32, 
                                              0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!sprite) return NULL;
    
    // Make transparent
    SDL_FillRect(sprite, NULL, SDL_MapRGBA(sprite->format, 0, 0, 0, 0));
    
    // Knight sprite pattern
    Uint32 armor_color = get_color(sprite, 150, 150, 150);
    Uint32 helmet_color = get_color(sprite, 100, 100, 100);
    Uint32 plume_color = get_color(sprite, 200, 50, 50);
    Uint32 skin_color = get_color(sprite, 220, 180, 140);
    Uint32 sword_color = get_color(sprite, 200, 200, 200);
    
    // Helmet
    for (int x = 5; x < 11; x++) {
        for (int y = 2; y < 6; y++) {
            draw_pixel(sprite, x, y, helmet_color);
        }
    }
    
    // Plume
    for (int x = 7; x < 9; x++) {
        draw_pixel(sprite, x, 1, plume_color);
    }
    
    // Face
    for (int x = 6; x < 10; x++) {
        draw_pixel(sprite, x, 5, skin_color);
    }
    
    // Body armor
    for (int x = 4; x < 12; x++) {
        for (int y = 6; y < 12; y++) {
            draw_pixel(sprite, x, y, armor_color);
        }
    }
    
    // Arms
    for (int y = 7; y < 10; y++) {
        draw_pixel(sprite, 3, y, armor_color);
        draw_pixel(sprite, 12, y, armor_color);
    }
    
    // Sword
    for (int y = 4; y < 8; y++) {
        draw_pixel(sprite, 13, y, sword_color);
    }
    draw_pixel(sprite, 12, 8, sword_color);
    draw_pixel(sprite, 13, 8, sword_color);
    draw_pixel(sprite, 14, 8, sword_color);
    
    // Legs
    for (int x = 5; x < 7; x++) {
        for (int y = 12; y < 16; y++) {
            draw_pixel(sprite, x, y, armor_color);
        }
    }
    for (int x = 9; x < 11; x++) {
        for (int y = 12; y < 16; y++) {
            draw_pixel(sprite, x, y, armor_color);
        }
    }
    
    return sprite;
}

SDL_Surface* create_wizard_sprite() {
    const int size = 16;
    SDL_Surface* sprite = SDL_CreateRGBSurface(0, size, size, 32, 
                                              0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!sprite) return NULL;
    
    SDL_FillRect(sprite, NULL, SDL_MapRGBA(sprite->format, 0, 0, 0, 0));
    
    Uint32 robe_color = get_color(sprite, 50, 50, 150);
    Uint32 hat_color = get_color(sprite, 75, 75, 200);
    Uint32 skin_color = get_color(sprite, 220, 180, 140);
    Uint32 staff_color = get_color(sprite, 139, 69, 19);
    Uint32 orb_color = get_color(sprite, 255, 255, 100);
    
    // Hat
    for (int x = 5; x < 11; x++) {
        for (int y = 1; y < 5; y++) {
            draw_pixel(sprite, x, y, hat_color);
        }
    }
    
    // Hat tip
    draw_pixel(sprite, 7, 0, hat_color);
    draw_pixel(sprite, 8, 0, hat_color);
    
    // Face
    for (int x = 6; x < 10; x++) {
        draw_pixel(sprite, x, 5, skin_color);
    }
    
    // Robe
    for (int x = 4; x < 12; x++) {
        for (int y = 6; y < 16; y++) {
            draw_pixel(sprite, x, y, robe_color);
        }
    }
    
    // Staff
    for (int y = 2; y < 12; y++) {
        draw_pixel(sprite, 2, y, staff_color);
    }
    
    // Orb
    draw_pixel(sprite, 1, 2, orb_color);
    draw_pixel(sprite, 2, 1, orb_color);
    draw_pixel(sprite, 3, 2, orb_color);
    draw_pixel(sprite, 2, 3, orb_color);
    
    return sprite;
}

SDL_Surface* create_monster_sprite(int type) {
    const int size = 16;
    SDL_Surface* sprite = SDL_CreateRGBSurface(0, size, size, 32, 
                                              0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!sprite) return NULL;
    
    SDL_FillRect(sprite, NULL, SDL_MapRGBA(sprite->format, 0, 0, 0, 0));
    
    switch (type) {
        case 0: { // Goblin
            Uint32 skin_color = get_color(sprite, 100, 150, 100);
            Uint32 eye_color = get_color(sprite, 255, 0, 0);
            Uint32 weapon_color = get_color(sprite, 139, 69, 19);
            
            // Body
            for (int x = 5; x < 11; x++) {
                for (int y = 6; y < 14; y++) {
                    draw_pixel(sprite, x, y, skin_color);
                }
            }
            
            // Head
            for (int x = 6; x < 10; x++) {
                for (int y = 3; y < 7; y++) {
                    draw_pixel(sprite, x, y, skin_color);
                }
            }
            
            // Eyes
            draw_pixel(sprite, 7, 4, eye_color);
            draw_pixel(sprite, 8, 4, eye_color);
            
            // Weapon
            for (int y = 5; y < 9; y++) {
                draw_pixel(sprite, 12, y, weapon_color);
            }
            break;
        }
        
        case 1: { // Orc
            Uint32 skin_color = get_color(sprite, 80, 100, 80);
            Uint32 armor_color = get_color(sprite, 100, 50, 50);
            Uint32 weapon_color = get_color(sprite, 150, 150, 150);
            
            // Body
            for (int x = 4; x < 12; x++) {
                for (int y = 7; y < 15; y++) {
                    draw_pixel(sprite, x, y, armor_color);
                }
            }
            
            // Head
            for (int x = 5; x < 11; x++) {
                for (int y = 2; y < 8; y++) {
                    draw_pixel(sprite, x, y, skin_color);
                }
            }
            
            // Tusks
            draw_pixel(sprite, 6, 6, get_color(sprite, 255, 255, 255));
            draw_pixel(sprite, 9, 6, get_color(sprite, 255, 255, 255));
            
            // Axe
            for (int y = 3; y < 7; y++) {
                draw_pixel(sprite, 13, y, weapon_color);
            }
            draw_pixel(sprite, 14, 4, weapon_color);
            draw_pixel(sprite, 14, 5, weapon_color);
            break;
        }
        
        case 2: { // Skeleton
            Uint32 bone_color = get_color(sprite, 240, 240, 200);
            Uint32 eye_color = get_color(sprite, 255, 0, 0);
            
            // Skull
            for (int x = 6; x < 10; x++) {
                for (int y = 2; y < 6; y++) {
                    draw_pixel(sprite, x, y, bone_color);
                }
            }
            
            // Eye sockets
            draw_pixel(sprite, 7, 3, eye_color);
            draw_pixel(sprite, 8, 3, eye_color);
            
            // Ribcage
            for (int x = 5; x < 11; x++) {
                for (int y = 6; y < 10; y++) {
                    if ((x + y) % 2 == 0) {
                        draw_pixel(sprite, x, y, bone_color);
                    }
                }
            }
            
            // Arms and legs (sparse)
            draw_pixel(sprite, 4, 7, bone_color);
            draw_pixel(sprite, 11, 7, bone_color);
            draw_pixel(sprite, 6, 12, bone_color);
            draw_pixel(sprite, 9, 12, bone_color);
            break;
        }
        
        case 3: { // Dragon
            Uint32 scale_color = get_color(sprite, 180, 0, 0);
            Uint32 wing_color = get_color(sprite, 120, 0, 0);
            Uint32 eye_color = get_color(sprite, 255, 255, 0);
            
            // Body
            for (int x = 2; x < 14; x++) {
                for (int y = 6; y < 12; y++) {
                    draw_pixel(sprite, x, y, scale_color);
                }
            }
            
            // Head
            for (int x = 4; x < 12; x++) {
                for (int y = 2; y < 7; y++) {
                    draw_pixel(sprite, x, y, scale_color);
                }
            }
            
            // Wings
            for (int x = 1; x < 4; x++) {
                for (int y = 4; y < 8; y++) {
                    draw_pixel(sprite, x, y, wing_color);
                }
            }
            for (int x = 12; x < 15; x++) {
                for (int y = 4; y < 8; y++) {
                    draw_pixel(sprite, x, y, wing_color);
                }
            }
            
            // Eyes
            draw_pixel(sprite, 6, 3, eye_color);
            draw_pixel(sprite, 9, 3, eye_color);
            
            // Tail
            for (int x = 12; x < 16; x++) {
                for (int y = 10; y < 14; y++) {
                    if (x + y < 25) {
                        draw_pixel(sprite, x, y, scale_color);
                    }
                }
            }
            break;
        }
    }
    
    return sprite;
}

SDL_Surface* create_dragon_sprite() {
    return create_monster_sprite(3);
}

SDL_Surface* create_treasure_sprite() {
    const int size = 16;
    SDL_Surface* sprite = SDL_CreateRGBSurface(0, size, size, 32, 
                                              0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!sprite) return NULL;
    
    SDL_FillRect(sprite, NULL, SDL_MapRGBA(sprite->format, 0, 0, 0, 0));
    
    Uint32 gold_color = get_color(sprite, 255, 215, 0);
    Uint32 dark_gold = get_color(sprite, 218, 165, 32);
    Uint32 gem_color = get_color(sprite, 255, 0, 255);
    
    // Chest base
    for (int x = 3; x < 13; x++) {
        for (int y = 8; y < 14; y++) {
            draw_pixel(sprite, x, y, dark_gold);
        }
    }
    
    // Chest lid
    for (int x = 3; x < 13; x++) {
        for (int y = 5; y < 9; y++) {
            draw_pixel(sprite, x, y, gold_color);
        }
    }
    
    // Lock
    draw_pixel(sprite, 7, 10, get_color(sprite, 100, 100, 100));
    draw_pixel(sprite, 8, 10, get_color(sprite, 100, 100, 100));
    
    // Gems spilling out
    draw_pixel(sprite, 5, 4, gem_color);
    draw_pixel(sprite, 10, 3, gem_color);
    draw_pixel(sprite, 12, 5, gem_color);
    
    return sprite;
}

SDL_Surface* create_potion_sprite() {
    const int size = 16;
    SDL_Surface* sprite = SDL_CreateRGBSurface(0, size, size, 32, 
                                              0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!sprite) return NULL;
    
    SDL_FillRect(sprite, NULL, SDL_MapRGBA(sprite->format, 0, 0, 0, 0));
    
    Uint32 bottle_color = get_color(sprite, 200, 200, 255);
    Uint32 liquid_color = get_color(sprite, 255, 100, 100);
    Uint32 cork_color = get_color(sprite, 139, 69, 19);
    
    // Bottle
    for (int x = 6; x < 10; x++) {
        for (int y = 6; y < 13; y++) {
            draw_pixel(sprite, x, y, bottle_color);
        }
    }
    
    // Neck
    for (int x = 7; x < 9; x++) {
        for (int y = 4; y < 7; y++) {
            draw_pixel(sprite, x, y, bottle_color);
        }
    }
    
    // Cork
    for (int x = 6; x < 10; x++) {
        for (int y = 2; y < 5; y++) {
            draw_pixel(sprite, x, y, cork_color);
        }
    }
    
    // Liquid
    for (int x = 7; x < 9; x++) {
        for (int y = 8; y < 12; y++) {
            draw_pixel(sprite, x, y, liquid_color);
        }
    }
    
    return sprite;
}