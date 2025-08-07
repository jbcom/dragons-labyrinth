#include "game.h"

// Audio parameters
#define SAMPLE_RATE 44100
#define AUDIO_FORMAT AUDIO_S16SYS
#define CHANNELS 2
#define CHUNK_SIZE 1024
#define MUSIC_LENGTH (SAMPLE_RATE * 10) // 10 seconds loop
#define SOUND_LENGTH (SAMPLE_RATE / 4)  // 0.25 seconds

// Generate a simple wave
Sint16 generate_wave(double frequency, double time, double amplitude) {
    return (Sint16)(amplitude * sin(2.0 * M_PI * frequency * time));
}

// Create synthesized background music
Mix_Music* create_background_music() {
    // Create a simple melody with multiple notes
    Sint16* music_buffer = malloc(MUSIC_LENGTH * CHANNELS * sizeof(Sint16));
    if (!music_buffer) return NULL;
    
    double notes[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25}; // C major scale
    int melody[] = {0, 2, 4, 5, 4, 2, 0, -1, 7, 5, 4, 2, 0, -1, 0, 2, 4, 5, 7, 7, 7, -1}; // Simple melody pattern
    int melody_length = sizeof(melody) / sizeof(melody[0]);
    
    double note_duration = (double)MUSIC_LENGTH / (SAMPLE_RATE * melody_length);
    
    for (int i = 0; i < MUSIC_LENGTH; i++) {
        double time = (double)i / SAMPLE_RATE;
        Sint16 sample = 0;
        
        // Determine which note to play
        int note_index = (int)(time / note_duration) % melody_length;
        if (melody[note_index] >= 0) {
            double frequency = notes[melody[note_index]];
            
            // Main melody
            sample += generate_wave(frequency, time, 8000);
            
            // Harmony (fifth)
            sample += generate_wave(frequency * 1.5, time, 4000);
            
            // Bass line (octave lower)
            sample += generate_wave(frequency * 0.5, time, 6000);
        }
        
        // Add some reverb/echo effect
        if (i > SAMPLE_RATE / 8) {
            sample += music_buffer[(i - SAMPLE_RATE/8) * CHANNELS] / 4;
        }
        
        // Stereo channels
        music_buffer[i * CHANNELS] = sample;
        music_buffer[i * CHANNELS + 1] = sample;
    }
    
    // Create SDL_RWops from buffer
    SDL_RWops* rw = SDL_RWFromMem(music_buffer, MUSIC_LENGTH * CHANNELS * sizeof(Sint16));
    if (!rw) {
        free(music_buffer);
        return NULL;
    }
    
    // Note: In a real implementation, we'd need to convert to a proper audio format
    // For now, we'll return NULL and use a simple approach
    SDL_FreeRW(rw);
    free(music_buffer);
    return NULL;
}

// Create combat sound effect
Mix_Chunk* create_combat_sound() {
    Sint16* sound_buffer = malloc(SOUND_LENGTH * CHANNELS * sizeof(Sint16));
    if (!sound_buffer) return NULL;
    
    for (int i = 0; i < SOUND_LENGTH; i++) {
        double time = (double)i / SAMPLE_RATE;
        Sint16 sample = 0;
        
        // Sword clash sound - mix of different frequencies with decay
        double decay = exp(-time * 8.0); // Exponential decay
        
        // High frequency component (metallic ring)
        sample += generate_wave(800 + 200 * sin(time * 50), time, 15000 * decay);
        
        // Mid frequency component
        sample += generate_wave(400 + 100 * sin(time * 30), time, 10000 * decay);
        
        // Low frequency component (impact)
        sample += generate_wave(150 + 50 * sin(time * 20), time, 8000 * decay);
        
        // Add some noise for realistic effect
        sample += (rand() % 2000 - 1000) * decay;
        
        // Stereo channels
        sound_buffer[i * CHANNELS] = sample;
        sound_buffer[i * CHANNELS + 1] = sample;
    }
    
    Mix_Chunk* chunk = malloc(sizeof(Mix_Chunk));
    if (chunk) {
        chunk->abuf = (Uint8*)sound_buffer;
        chunk->alen = SOUND_LENGTH * CHANNELS * sizeof(Sint16);
        chunk->volume = MIX_MAX_VOLUME;
    } else {
        free(sound_buffer);
    }
    
    return chunk;
}

// Create pickup sound effect
Mix_Chunk* create_pickup_sound() {
    Sint16* sound_buffer = malloc(SOUND_LENGTH * CHANNELS * sizeof(Sint16));
    if (!sound_buffer) return NULL;
    
    for (int i = 0; i < SOUND_LENGTH; i++) {
        double time = (double)i / SAMPLE_RATE;
        Sint16 sample = 0;
        
        // Pleasant chime sound - ascending notes
        double frequency = 523.25 + (time * 200); // Start at C5 and rise
        double envelope = exp(-time * 4.0) * sin(time * 20); // Bell-like envelope
        
        sample += generate_wave(frequency, time, 12000 * envelope);
        sample += generate_wave(frequency * 2, time, 6000 * envelope); // Octave harmonic
        sample += generate_wave(frequency * 3, time, 3000 * envelope); // Third harmonic
        
        // Stereo channels
        sound_buffer[i * CHANNELS] = sample;
        sound_buffer[i * CHANNELS + 1] = sample;
    }
    
    Mix_Chunk* chunk = malloc(sizeof(Mix_Chunk));
    if (chunk) {
        chunk->abuf = (Uint8*)sound_buffer;
        chunk->alen = SOUND_LENGTH * CHANNELS * sizeof(Sint16);
        chunk->volume = MIX_MAX_VOLUME;
    } else {
        free(sound_buffer);
    }
    
    return chunk;
}

int init_audio(Game* game) {
    if (Mix_OpenAudio(SAMPLE_RATE, AUDIO_FORMAT, CHANNELS, CHUNK_SIZE) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }
    
    // Create audio effects
    game->combat_sound = create_combat_sound();
    game->pickup_sound = create_pickup_sound();
    
    // For background music, we'll use a simple approach since creating
    // a proper music file format is complex
    game->bg_music = NULL;
    
    return 0;
}

void play_combat_sound(Game* game) {
    if (game->combat_sound) {
        Mix_PlayChannel(-1, game->combat_sound, 0);
    }
}

void play_pickup_sound(Game* game) {
    if (game->pickup_sound) {
        Mix_PlayChannel(-1, game->pickup_sound, 0);
    }
}