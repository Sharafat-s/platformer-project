#include "enemies_controller.h"
#include "globals.h"
#include "level_manager.h"
#include "level.h"

void EnemiesController::spawn_enemies() {
    enemies.clear();

    Level &level = Level::get_instance();
    int rows = level.get_current_level().rows;
    int columns = level.get_current_level().columns;

    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            char &cell = level.get_level_cell(row, column);
            if (cell == ENEMY) {
                enemies.push_back({
                    {static_cast<float>(column), static_cast<float>(row)},
                    true
                });
                cell = AIR;
            }
        }
    }
}

void EnemiesController::update_enemies() {
    for (auto &enemy: enemies) {
        // Find the enemy's next x
        float next_x = enemy.get_pos().x;
        next_x += (enemy.is_looking_right() ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED);

        // If its next position collides with a wall, turn around
        if (Level::get_instance().is_colliding({next_x, enemy.get_pos().y}, WALL)) {
            enemy.set_looking_right(!enemy.is_looking_right());
        }
        // Otherwise, keep moving
        else {
            enemy.set_pos(Vector2{next_x, enemy.get_pos().y});
        }
    }
}

bool EnemiesController::is_colliding_with_enemies(const Vector2 pos) const {
    const Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto &enemy: enemies) {
        Rectangle enemy_hitbox = {enemy.get_pos().x, enemy.get_pos().y, 1.0f, 1.0f};
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            return true;
        }
    }
    return false;
}

void EnemiesController::remove_colliding_enemy(const Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        Rectangle enemy_hitbox = {it->get_pos().x, it->get_pos().y, 1.0f, 1.0f};
        // Erase a colliding enemy
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            enemies.erase(it);


            remove_colliding_enemy(pos);
            return;
        }
    }
}
