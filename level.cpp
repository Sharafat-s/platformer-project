// level.cpp
#include <iostream>
#include <stdexcept>
#include <raylib.h>
#include "level.h"
#include "parser.h"
#include "graphics.h"
#include "level_manager.h"

Level::Level() = default;
Level::~Level() {
    delete[] current_level_data;
}

// Singleton accessor
Level& Level::get_instance() {
    static Level instance;
    return instance;
}

int Level::get_level_index() const {
    return level_index;
}

void Level::set_level_index(int index) {
    level_index = index;
}

level Level::get_current_level() const {
    return current_level;
}

char* Level::get_current_level_data() {
    return current_level_data;
}

bool Level::load_all_levels(const std::string& filepath) {
    levels = Parser::parseRLL(filepath);
    if (levels.empty()) {
        TraceLog(LOG_ERROR, "Failed to load any levels from file: %s", filepath.c_str());
        return false;
    }
    return !levels.empty();
}

// void Level::init_levels(const std::string& filepath) {
//     levels = Parser::parseRLL(filepath);
//     LevelManager::get_level().set_level_count(static_cast<int>(levels.size()));
//
//     TraceLog(LOG_INFO, "Levels loaded: %zu", levels.size());
//
//     if (levels.empty()) {
//         TraceLog(LOG_FATAL, "No levels loaded from %s", filepath.c_str());
//         exit(EXIT_FAILURE);
//     }
//
//     level_index = 0;
//     load_level();
// }
//
// void Level::load_level(int offset) {
//     std::cout << "Level dimensions: "
//           << LevelManager::get_level().get_current_level().rows
//           << "x"
//           << LevelManager::get_level().get_current_level().columns
//           << std::endl;
//
//     if (levels.empty()) return;

void Level::init_levels(const std::string& filepath) {
    levels = Parser::parseRLL(filepath);
    LevelManager::get_instance().set_level_count(static_cast<int>(levels.size()));

    TraceLog(LOG_INFO, "Levels loaded: %zu", levels.size());

    if (levels.empty()) {
        TraceLog(LOG_FATAL, "No levels loaded from %s", filepath.c_str());
        exit(EXIT_FAILURE);
    }

    level_index = 0;
    load_level();
}

void Level::load_level(int offset) {
    std::cout << "Level dimensions: "
              << get_current_level().rows
              << "x"
              << get_current_level().columns
              << std::endl;

    if (levels.empty()) return;
    level_index += offset;
    if (level_index < 0) level_index = 0;
    if (level_index >= static_cast<int>(levels.size())) level_index = static_cast<int>(levels.size()) - 1;

    auto& lines = levels[level_index];
    size_t rows = lines.size();
    size_t cols = lines[0].size();

    delete[] current_level_data;
    current_level_data = new char[rows * cols];

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            current_level_data[r * cols + c] = lines[r][c];
        }
    }

    current_level.rows = static_cast<int>(rows);
    current_level.columns = static_cast<int>(cols);
}

void Level::unload_level() {
    delete[] current_level_data;
    current_level_data = nullptr;
    current_level = {0, 0};
}

bool Level::is_inside_level(int row, int col) {
    Level& lvl = get_instance();
    return row >= 0 && row < lvl.current_level.rows && col >= 0 && col < lvl.current_level.columns;
}

bool Level::is_colliding(Vector2 pos, char look_for) {
    Level& lvl = get_instance();
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (int row = static_cast<int>(pos.y) - 1; row <= static_cast<int>(pos.y) + 1; ++row) {
        for (int col = static_cast<int>(pos.x) - 1; col <= static_cast<int>(pos.x) + 1; ++col) {
            if (!is_inside_level(row, col)) continue;
            if (lvl.get_level_cell(row, col) == look_for) {
                Rectangle block_hitbox = {static_cast<float>(col), static_cast<float>(row), 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) return true;
            }
        }
    }
    return false;
}

char& Level::get_collider(Vector2 pos, char look_for) {
    for (int row = static_cast<int>(pos.y) - 1; row <= static_cast<int>(pos.y) + 1; ++row) {
        for (int col = static_cast<int>(pos.x) - 1; col <= static_cast<int>(pos.x) + 1; ++col) {
            if (!is_inside_level(row, col)) continue;
            if (get_level_cell(row, col) == look_for) {
                Rectangle block_hitbox = {static_cast<float>(col), static_cast<float>(row), 1.0f, 1.0f};
                if (CheckCollisionRecs({pos.x, pos.y, 1.0f, 1.0f}, block_hitbox)) {
                    return get_level_cell(row, col);
                }
            }
        }
    }
    return get_level_cell(static_cast<int>(pos.y), static_cast<int>(pos.x));
}

char& Level::get_level_cell(size_t row, size_t column) {
    Level& lvl = get_instance();
    if (!lvl.current_level_data) {
        TraceLog(LOG_FATAL, "Accessed level cell but data is nullptr!");
        exit(EXIT_FAILURE);
    }
    return lvl.current_level_data[row * lvl.current_level.columns + column];
}

void Level::set_level_cell(size_t row, size_t column, char chr) {
    Level& lvl = get_instance();
    if (!lvl.current_level_data || row >= static_cast<size_t>(lvl.current_level.rows) || column >= static_cast<size_t>(lvl.current_level.columns)) {
        throw std::out_of_range("Level cell access out of bounds");
    }
    lvl.current_level_data[row * lvl.current_level.columns + column] = chr;
}

void Level::reset_level_index() {
    get_instance().level_index = 0;
}

void Level::load_level_from_lines(const std::vector<std::string>& lines) {
    size_t rows = lines.size();
    size_t cols = lines[0].size();

    delete[] current_level_data;
    current_level_data = new char[rows * cols];

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            current_level_data[r * cols + c] = lines[r][c];
        }
    }

    current_level.rows = static_cast<int>(rows);
    current_level.columns = static_cast<int>(cols);
}


