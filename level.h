#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "raylib.h"
#include "player.h"

/*
// External state
extern enum game_state;
extern const enum VICTORY_STATE;
extern void create_victory_menu_background();
extern void derive_graphics_metrics_from_loaded_level();
extern int timer;
extern const int MAX_LEVEL_TIME;

class Level {
private:
    int levelIndex = 0;
    level levels[LEVEL_COUNT] = { LEVEL_1, LEVEL_2, LEVEL_3 };

    level currentLevel{};
    char* currentLevelData = nullptr;

    Level() = default;
    ~Level() {
        unload();
    }

public:
    static Level& get_instance() {
        static Level instance;
        return instance;
    }

    Level(const Level&) = delete;
    void operator=(const Level&) = delete;

    bool is_inside(int row, int column) const {
        return row >= 0 && row < static_cast<int>(currentLevel.rows) &&
               column >= 0 && column < static_cast<int>(currentLevel.columns);
    }

    char& get_cell(size_t row, size_t column) {
        return currentLevel.data[row * currentLevel.columns + column];
    }

    void set_cell(size_t row, size_t column, char chr) {
        get_cell(row, column) = chr;
    }

    const level& get_current_level() const {
        return currentLevel;
    }

    int get_rows() const {
        return currentLevel.rows;
    }

    int get_columns() const {
        return currentLevel.columns;
    }

    bool is_colliding(Vector2 pos, char look_for) {
        Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

        for (int row = static_cast<int>(pos.y) - 1; row < static_cast<int>(pos.y) + 1; ++row) {
            for (int col = static_cast<int>(pos.x) - 1; col < static_cast<int>(pos.x) + 1; ++col) {
                if (!is_inside(row, col)) continue;
                if (get_cell(row, col) == look_for) {
                    Rectangle block_hitbox = {(float)col, (float)row, 1.0f, 1.0f};
                    if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    char& get_collider(Vector2 pos, char look_for) {
        Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

        for (int row = static_cast<int>(pos.y) - 1; row < static_cast<int>(pos.y) + 1; ++row) {
            for (int col = static_cast<int>(pos.x) - 1; col < static_cast<int>(pos.x) + 1; ++col) {
                if (!is_inside(row, col)) continue;
                if (get_cell(row, col) == look_for) {
                    Rectangle block_hitbox = {(float)col, (float)row, 1.0f, 1.0f};
                    if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                        return get_cell(row, col);
                    }
                }
            }
        }

        // If failed, return a reference to the player's current cell
        return get_cell(pos.y, pos.x);
    }

    void reset_index() {
        levelIndex = 0;
    }

    void load(int offset) {
        unload(); // Unload previous level if loaded

        levelIndex += offset;

        if (levelIndex >= LEVEL_COUNT) {
            game_state = VICTORY_STATE;
            create_victory_menu_background();
            levelIndex = 0;
            return;
        }

        size_t rows = levels[levelIndex].rows;
        size_t cols = levels[levelIndex].columns;

        currentLevelData = new char[rows * cols];
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                currentLevelData[r * cols + c] = levels[levelIndex].data[r * cols + c];
            }
        }

        currentLevel = { rows, cols, currentLevelData };

        Player::get_instance().spawn_player();
        EnemiesController::get_instance().spawn_enemies();
        derive_graphics_metrics_from_loaded_level();

        timer = MAX_LEVEL_TIME;
    }

    void unload() {
        delete[] currentLevelData;
        currentLevelData = nullptr;
    }
};
*/
/*
class Level {
private:
    int levelIndex = 0;
    static constexpr int LEVEL_COUNT = 3;
    level levels[LEVEL_COUNT] = {LEVEL_1, LEVEL_2, LEVEL_3};

    level currentLevel{};
    char* currentLevelData = nullptr;

    Level() = default;
    ~Level() {
        unload();
    }

public:
    static Level& get_instance() {
        static Level instance;
        return instance;
    }

    Level(const Level&) = delete;
    void operator=(const Level&) = delete;

    bool is_inside(int row, int column) const {
        return row >= 0 && row < currentLevel.rows && column >= 0 && column < currentLevel.columns;
    }

    char& get_cell(size_t row, size_t column) {
        return currentLevel.data[row * currentLevel.columns + column];
    }

    void set_cell(size_t row, size_t column, char chr) {
        get_cell(row, column) = chr;
    }

    bool is_colliding(Vector2 pos, char look_for) {
        Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

        for (int row = static_cast<int>(pos.y) - 1; row < static_cast<int>(pos.y) + 1; ++row) {
            for (int col = static_cast<int>(pos.x) - 1; col < static_cast<int>(pos.x) + 1; ++col) {
                if (!is_inside(row, col)) continue;
                if (get_cell(row, col) == look_for) {
                    Rectangle block_hitbox = {(float)col, (float)row, 1.0f, 1.0f};
                    if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    char& get_collider(Vector2 pos, char look_for) {
        Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

        for (int row = static_cast<int>(pos.y) - 1; row < static_cast<int>(pos.y) + 1; ++row) {
            for (int col = static_cast<int>(pos.x) - 1; col < static_cast<int>(pos.x) + 1; ++col) {
                if (!is_inside(row, col)) continue;
                if (get_cell(row, col) == look_for) {
                    Rectangle block_hitbox = {(float)col, (float)row, 1.0f, 1.0f};
                    if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                        return get_cell(row, col);
                    }
                }
            }
        }

        // Return approximate cell if no collision found
        return get_cell(pos.y, pos.x);
    }

    void reset_index() {
        levelIndex = 0;
    }

    void load(int offset) {
        unload(); // Unload previous level

        levelIndex += offset;

        // Win logic
        if (levelIndex >= LEVEL_COUNT) {
            game_state = VICTORY_STATE;
            create_victory_menu_background();
            levelIndex = 0;
            return;
        }

        size_t rows = levels[levelIndex].rows;
        size_t cols = levels[levelIndex].columns;
        currentLevelData = new char[rows * cols];

        for (size_t row = 0; row < rows; ++row) {
            for (size_t col = 0; col < cols; ++col) {
                currentLevelData[row * cols + col] = levels[levelIndex].data[row * cols + col];
            }
        }

        currentLevel = {rows, cols, currentLevelData};

        Player::get_instance().spawn_player();
        EnemiesController::get_instance().spawn_enemies();
        derive_graphics_metrics_from_loaded_level();

        timer = MAX_LEVEL_TIME;
    }

    void unload() {
        delete[] currentLevelData;
        currentLevelData = nullptr;
    }
};
*/

bool is_inside_level(int row, int column) {
    if (row < 0 || row >= current_level.rows) return false;
    if (column < 0 || column >= current_level.columns) return false;
    return true;
}

bool is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    // Scan the adjacent area in the level to look for a match in collision
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& get_collider(Vector2 pos, char look_for) {
    // Like is_colliding(), except returns a reference to the colliding object
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_level_cell(row, column);
                }
            }
        }
    }

    // If failed, get an approximation
    return get_level_cell(pos.x, pos.y);;
}

void reset_level_index() {
    level_index = 0;
}

void load_level(int offset) {
    level_index += offset;

    // Win logic
    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    // Level duplication
    size_t rows = LEVELS[level_index].rows;
    size_t columns = LEVELS[level_index].columns;
    current_level_data = new char[rows*columns];

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            current_level_data[row * columns + column] = LEVELS[level_index].data[row * columns + column];
        }
    }

    current_level = {rows, columns, current_level_data};

    // Instantiate entities
    Player::get_instance().spawn_player();
    EnemiesController::get_instance().spawn_enemies();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

void unload_level() {
    delete[] current_level_data;
}

// Getters and setters
char& get_level_cell(size_t row, size_t column) {
    return current_level.data[row * current_level.columns + column];
}

void set_level_cell(size_t row, size_t column, char chr) {
    get_level_cell(row, column) = chr;
}

#endif //LEVEL_H
