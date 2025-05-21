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

    std::cout << "[LevelManager::init] levels loaded = " << get_instance().levels.size() << std::endl;

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
