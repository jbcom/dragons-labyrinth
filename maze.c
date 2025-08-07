#include "game.h"

typedef struct {
    int x, y;
} Point;

static bool visited[MAZE_HEIGHT][MAZE_WIDTH];
static Point stack[MAZE_HEIGHT * MAZE_WIDTH];
static int stack_top = -1;

void push(Point p) {
    if (stack_top < MAZE_HEIGHT * MAZE_WIDTH - 1) {
        stack[++stack_top] = p;
    }
}

Point pop() {
    if (stack_top >= 0) {
        return stack[stack_top--];
    }
    return (Point){-1, -1};
}

bool is_empty() {
    return stack_top < 0;
}

bool is_valid_cell(int x, int y) {
    return x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT;
}

bool has_unvisited_neighbors(int x, int y) {
    int dx[] = {0, 0, 2, -2};
    int dy[] = {2, -2, 0, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (is_valid_cell(nx, ny) && !visited[ny][nx]) {
            return true;
        }
    }
    return false;
}

Point get_random_unvisited_neighbor(int x, int y) {
    Point neighbors[4];
    int count = 0;
    
    int dx[] = {0, 0, 2, -2};
    int dy[] = {2, -2, 0, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (is_valid_cell(nx, ny) && !visited[ny][nx]) {
            neighbors[count++] = (Point){nx, ny};
        }
    }
    
    if (count > 0) {
        return neighbors[rand() % count];
    }
    return (Point){-1, -1};
}

void carve_passage(TileType maze[MAZE_HEIGHT][MAZE_WIDTH], int x1, int y1, int x2, int y2) {
    maze[y1][x1] = TILE_FLOOR;
    maze[y2][x2] = TILE_FLOOR;
    
    // Carve the wall between them
    int mx = (x1 + x2) / 2;
    int my = (y1 + y2) / 2;
    maze[my][mx] = TILE_FLOOR;
}

void generate_maze(TileType maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    // Initialize maze with walls
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            maze[y][x] = TILE_WALL;
            visited[y][x] = false;
        }
    }
    
    // Reset stack
    stack_top = -1;
    
    // Start from (1, 1)
    Point start = {1, 1};
    visited[start.y][start.x] = true;
    maze[start.y][start.x] = TILE_FLOOR;
    push(start);
    
    while (!is_empty()) {
        Point current = stack[stack_top]; // Peek at top
        
        if (has_unvisited_neighbors(current.x, current.y)) {
            Point next = get_random_unvisited_neighbor(current.x, current.y);
            if (next.x != -1) {
                visited[next.y][next.x] = true;
                carve_passage(maze, current.x, current.y, next.x, next.y);
                push(next);
            }
        } else {
            pop();
        }
    }
    
    // Ensure start position is clear
    maze[1][1] = TILE_FLOOR;
    
    // Create some additional openings for better gameplay
    for (int i = 0; i < 20; i++) {
        int x = 2 + (rand() % (MAZE_WIDTH - 4));
        int y = 2 + (rand() % (MAZE_HEIGHT - 4));
        if (maze[y][x] == TILE_WALL) {
            // Check if removing this wall connects two floor areas
            int floor_neighbors = 0;
            if (maze[y-1][x] == TILE_FLOOR) floor_neighbors++;
            if (maze[y+1][x] == TILE_FLOOR) floor_neighbors++;
            if (maze[y][x-1] == TILE_FLOOR) floor_neighbors++;
            if (maze[y][x+1] == TILE_FLOOR) floor_neighbors++;
            
            if (floor_neighbors >= 2) {
                maze[y][x] = TILE_FLOOR;
            }
        }
    }
}

void place_entities(GameWorld* world) {
    // Clear existing entities
    world->monster_count = 0;
    world->potion_count = 0;
    world->treasure_found = false;
    world->dragon_defeated = false;
    
    // Find all floor tiles
    Point floor_tiles[MAZE_WIDTH * MAZE_HEIGHT];
    int floor_count = 0;
    
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (world->maze[y][x] == TILE_FLOOR && !(x == 1 && y == 1)) {
                floor_tiles[floor_count++] = (Point){x, y};
            }
        }
    }
    
    if (floor_count < 10) return; // Not enough space
    
    // Shuffle floor tiles
    for (int i = 0; i < floor_count; i++) {
        int j = rand() % floor_count;
        Point temp = floor_tiles[i];
        floor_tiles[i] = floor_tiles[j];
        floor_tiles[j] = temp;
    }
    
    int tile_index = 0;
    
    // Place treasure (far from start)
    int best_treasure_distance = 0;
    int best_treasure_index = 0;
    for (int i = 0; i < floor_count && i < 50; i++) {
        int distance = abs(floor_tiles[i].x - 1) + abs(floor_tiles[i].y - 1);
        if (distance > best_treasure_distance) {
            best_treasure_distance = distance;
            best_treasure_index = i;
        }
    }
    
    world->maze[floor_tiles[best_treasure_index].y][floor_tiles[best_treasure_index].x] = TILE_TREASURE;
    
    // Remove treasure tile from available tiles
    floor_tiles[best_treasure_index] = floor_tiles[--floor_count];
    
    // Place dragon (also far from start, but not at treasure)
    best_treasure_distance = 0;
    best_treasure_index = 0;
    for (int i = 0; i < floor_count && i < 50; i++) {
        int distance = abs(floor_tiles[i].x - 1) + abs(floor_tiles[i].y - 1);
        if (distance > best_treasure_distance) {
            best_treasure_distance = distance;
            best_treasure_index = i;
        }
    }
    
    world->maze[floor_tiles[best_treasure_index].y][floor_tiles[best_treasure_index].x] = TILE_DRAGON;
    
    // Create dragon monster
    world->monsters[world->monster_count] = (Monster){
        .x = floor_tiles[best_treasure_index].x,
        .y = floor_tiles[best_treasure_index].y,
        .hp = 200, .max_hp = 200,
        .attack = 50, .defense = 30,
        .type = 3, .alive = true,
        .sprite = create_monster_sprite(3)
    };
    world->monster_count++;
    
    // Remove dragon tile from available tiles
    floor_tiles[best_treasure_index] = floor_tiles[--floor_count];
    
    // Place regular monsters
    int monsters_to_place = (rand() % 6) + 4; // 4-9 monsters
    if (monsters_to_place > MAX_MONSTERS - 1) monsters_to_place = MAX_MONSTERS - 1;
    
    for (int i = 0; i < monsters_to_place && tile_index < floor_count; i++) {
        int monster_type = rand() % 3; // 0-2 for regular monsters
        
        world->monsters[world->monster_count] = (Monster){
            .x = floor_tiles[tile_index].x,
            .y = floor_tiles[tile_index].y,
            .hp = 30 + (monster_type * 20),
            .max_hp = 30 + (monster_type * 20),
            .attack = 10 + (monster_type * 5),
            .defense = 5 + (monster_type * 3),
            .type = monster_type,
            .alive = true,
            .sprite = create_monster_sprite(monster_type)
        };
        world->monster_count++;
        tile_index++;
    }
    
    // Place potions
    int potions_to_place = (rand() % 3) + 2; // 2-4 potions
    if (potions_to_place > MAX_POTIONS) potions_to_place = MAX_POTIONS;
    
    for (int i = 0; i < potions_to_place && tile_index < floor_count; i++) {
        world->potions[world->potion_count] = (Potion){
            .x = floor_tiles[tile_index].x,
            .y = floor_tiles[tile_index].y,
            .collected = false
        };
        world->potion_count++;
        tile_index++;
    }
}