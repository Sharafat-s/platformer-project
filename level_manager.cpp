#include "level_manager.h"
#include "parser.h"
#include "level.h"
#include "player.h"
#include "enemies_controller.h"
#include "graphics.h"

LevelManager& LevelManager::get_instance() {
    static LevelManager instance;
    return instance;
}

int LevelManager::get_index() {
    return get_instance().level_index;
}

int LevelManager::get_level_count() {
    return static_cast<int>(get_instance().levels.size());
}

void LevelManager::set_level_count(int count) {
    total_level_count = count;
}

const std::vector<std::string>& LevelManager::get_raw_level() {
    return get_instance().levels[get_instance().level_index];
}

void LevelManager::init(const std::string& filepath) {
    level_file = filepath;
    get_instance().levels = Parser::parseRLL(filepath);
    reload();
}

void LevelManager::reload() {
    Level::get_instance().load_level_from_lines(get_raw_level()); // 👈
    Player::get_instance().spawn_player();
    EnemiesController::get_instance().spawn_enemies();
    derive_graphics_metrics_from_loaded_level();
    timer = MAX_LEVEL_TIME;
}

void LevelManager::load_next() {
    LevelManager& lm = get_instance();
    lm.level_index++;
    if (lm.level_index >= get_level_count()) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        reset();
    } else {
        reload();
    }
}

void LevelManager::reset() {
    get_instance().level_index = 0;
    reload();
}

// #include "level_manager.h"
// #include "player.h"
// #include "enemies_controller.h"
// #include "graphics.h"
// #include "parser.h"
//
// LevelManager& LevelManager::get_instance() {
//     static LevelManager instance;
//     return instance;
// }
//
// int LevelManager::get_index() {
//     return get_instance().level_index;
// }
//
// auto& raw_level_lines = LevelManager::get_raw_level();
// Level::get_instance().load_from_lines(raw_level_lines);
//
// // Return current level data (e.g., dimensions)
// // std::vector<std::string> LevelManager::get_level() {
// //     return get_instance().levels[get_instance().level_index];
// // }
//
// int LevelManager::get_level_count() const {
//     return static_cast<int>(levels.size());
// }
//
// // void LevelManager::set_levels(const std::vector<level>& lvls) {
// //     levels = lvls;
// // }
// void LevelManager::set_levels(const std::vector<std::vector<std::string>>& lvls) {
//     get_instance().levels = lvls;
// }
//
//
// void LevelManager::init(const std::string& filepath) {
//     level_file = filepath;
//
//     level_file = filepath;
//
//     auto parsed_levels = Parser::parseRLL(filepath);
//     get_instance().set_levels(parsed_levels);
//     reload();
//
//     // // Load all level data from file into LevelManager's list
//     // get_instance().set_levels(Parser::parseRLL(filepath));
//     //
//     // // Load the first level into the Level singleton
//     // Level::get_instance().init_levels(filepath);
//     // reload();
// }
//
// void LevelManager::reload() {
//     Level::get_instance().load_level();  // loads level from index
//     Player::get_instance().spawn_player();
//     EnemiesController::get_instance().spawn_enemies();
//     derive_graphics_metrics_from_loaded_level();
//     timer = MAX_LEVEL_TIME;
// }
//
// void LevelManager::load_next() {
//     Level& level = Level::get_instance();
//     level.set_level_index(level.get_level_index() + 1);
//
//     //if (level.get_level_index() >= get_level_count()) {
//     if (level.get_level_index() >= get_instance().get_level_count()) {
//         game_state = VICTORY_STATE;
//         create_victory_menu_background();
//         reset();
//     } else {
//         reload();
//     }
// }
//
// void LevelManager::reset() {
//     Level::get_instance().reset_level_index();
//     reload();
// }

// #include "level_manager.h"
// #include "player.h"
// #include "enemies_controller.h"
// #include "graphics.h"
//
// LevelManager& LevelManager::get_instance() {
//     static LevelManager instance;
//     return instance;
// }
//
// int LevelManager::get_index() {
//     return get_instance().level_index;
// }
//
// level& LevelManager::get_level() {
//     return get_instance().levels[get_instance().level_index];
// }
//
// int LevelManager::get_level_count() const {
//     return static_cast<int>(levels.size());
// }
//
// void LevelManager::set_levels(const std::vector<level>& lvls) {
//     levels = lvls;
// }
//
// void LevelManager::init(const std::string& filepath) {
//     level_file = filepath;
//     Level::get_instance().init_levels(filepath);
//     reload();
// }
//
// void LevelManager::reload() {
//     Level::get_instance().load_level();
//     Player::get_instance().spawn_player();
//     EnemiesController::get_instance().spawn_enemies();
//     derive_graphics_metrics_from_loaded_level();
//     timer = MAX_LEVEL_TIME;
// }
//
// void LevelManager::load_next() {
//     Level& level = get_level();
//     level.set_level_index(level.get_level_index() + 1);
//
//     if (level.get_level_index() >= level.get_current_level().rows) {
//         game_state = VICTORY_STATE;
//         create_victory_menu_background();
//         reset();
//     } else {
//         reload();
//     }
// }
//
// void LevelManager::reset() {
//     get_level().reset_level_index();
//     reload();
// }

// #include "level_manager.h"
// #include "player.h"
// #include "enemies_controller.h"
// #include "graphics.h"
//
// LevelManager& LevelManager::get_instance() {
//     static LevelManager instance;
//     return instance;
// }
//
// int LevelManager::get_level_count() {
//     return static_cast<int>(levels.size()); // assuming levels is stored here
// }
//
// //
// // int LevelManager::get_level_count() const {
// //     return total_level_count;
// // }
// //
// // void LevelManager::set_level_count(int count) {
// //     total_level_count = count;
// // }
//
// void LevelManager::init(const std::string& filepath) {
//     level_file = filepath;
//     Level::get_instance().init_levels("data/levels.rll");
//     reload();
// }
//
// void LevelManager::reload() {
//     Level::get_instance().load_level();
//     Player::get_instance().spawn_player();
//     EnemiesController::get_instance().spawn_enemies();
//     derive_graphics_metrics_from_loaded_level();
//     timer = MAX_LEVEL_TIME;
// }
//
// // int LevelManager::get_level_count() {
// //     return static_cast<int>(get_instance().levels.size());
// // }
//
// int LevelManager::get_index() {
//     return level_index;
// }
//
//
// void LevelManager::load_next() {
//     Level& level = Level::get_instance();
//     level.set_level_index(level.get_level_index() + 1);
//
//     if (level.get_level_index() >= 3) {      //static_cast<int>(level.get_current_level().rows)) {
//         game_state = VICTORY_STATE;
//         create_victory_menu_background();
//         reset();
//     } else {
//         reload();
//     }
// }
//
// void LevelManager::reset() {
//     Level::get_instance().reset_level_index();
//     reload();
// }
//
// Level& LevelManager::get_level() {
//     return Level::get_instance();
// }
