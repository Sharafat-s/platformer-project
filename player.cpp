#include "player.h"
#include "enemies_controller.h"
#include <cmath>

// --- Singleton instance ---
Player& Player::get_instance() {
    static Player instance;
    return instance;
}

Player::Player() {}

void Player::reset_player_stats() {
    lives = MAX_LIVES;
    level_scores.fill(0);
}

void Player::increment_score() {
    PlaySound(coin_sound);
    level_scores[level_index]++;
}

int Player::get_total_player_score() const {
    int sum = 0;
    for (int score : level_scores)
        sum += score;
    return sum;
}

int Player::get_player_lives() const {
    return lives;
}

void Player::move_player_horizontally(float delta) {
    PlayerMovement::move_horizontally(*this, delta);
}

void Player::update_player_gravity() {
    PlayerMovement::update_gravity(*this);
}

void Player::update_player() {
    PlayerMovement::update(*this);
}

void Player::spawn_player() {
    y_velocity = 0;

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);
            if (cell == PLAYER) {
                position.x = column;
                position.y = row;
                set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

void Player::kill_player() {
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    lives--;
    level_scores[level_index] = 0;
}

Vector2 Player::get_player_pos() const { return position; }
void Player::set_position(Vector2 pos) { position = pos; }

float Player::get_player_y_velocity() const { return y_velocity; }
void Player::set_player_y_velocity(float vel) { y_velocity = vel; }

bool Player::is_player_on_ground() const { return on_ground; }
void Player::set_player_on_ground(bool val) { on_ground = val; }

bool Player::is_looking_forward() const { return looking_forward; }
void Player::set_looking_forward(bool val) { looking_forward = val; }

bool Player::is_moving() const { return moving; }
void Player::set_moving(bool val) { moving = val; }

int Player::get_time_to_coin_counter() const { return time_to_coin_counter; }
void Player::increment_time_to_coin_counter(int v) { time_to_coin_counter += v; }
void Player::reset_time_to_coin_counter() { time_to_coin_counter = 0; }

// PlayerMovement implementation
namespace PlayerMovement {

    void move_horizontally(Player& player, float delta) {
        float next_x = player.get_player_pos().x + delta;
        Vector2 new_pos = player.get_player_pos();

        if (!is_colliding({next_x, new_pos.y}, WALL)) {
            new_pos.x = next_x;
            player.set_position(new_pos);
        } else {
            new_pos.x = roundf(new_pos.x);
            player.set_position(new_pos);
            return;
        }

        player.set_looking_forward(delta > 0);
        if (delta != 0)
            player.set_moving(true);
    }

    void update_gravity(Player& player) {
        Vector2 pos = player.get_player_pos();
        float vel = player.get_player_y_velocity();

        if (is_colliding({pos.x, pos.y - 0.1f}, WALL) && vel < 0)
            vel = CEILING_BOUNCE_OFF;

        pos.y += vel;
        vel += GRAVITY_FORCE;

        bool on_ground = is_colliding({pos.x, pos.y + 0.1f}, WALL);
        if (on_ground) {
            vel = 0;
            pos.y = roundf(pos.y);
        }

        player.set_position(pos);
        player.set_player_y_velocity(vel);
        player.set_player_on_ground(on_ground);
    }

    void update(Player& player) {
        update_gravity(player);
        Vector2 pos = player.get_player_pos();

        if (is_colliding(pos, COIN)) {
            get_collider(pos, COIN) = AIR;
            player.increment_score();
        }

        if (is_colliding(pos, EXIT)) {
            if (timer > 0) {
                timer -= 25;
                player.increment_time_to_coin_counter(5);
                if (player.get_time_to_coin_counter() / 60 > 1) {
                    player.increment_score();
                    player.reset_time_to_coin_counter();
                }
            } else {
                load_level(1);
                PlaySound(exit_sound);
            }
        } else {
            if (timer >= 0) timer--;
        }

        if (is_colliding(pos, SPIKE) || pos.y > current_level.rows) {
            player.kill_player();
        }

        if (EnemiesController::get_instance().is_colliding_with_enemies(pos)) {
            if (player.get_player_y_velocity() > 0) {
                EnemiesController::get_instance().remove_colliding_enemy(pos);
                PlaySound(kill_enemy_sound);
                player.increment_score();
                player.set_player_y_velocity(-BOUNCE_OFF_ENEMY);
            } else {
                player.kill_player();
            }
        }
    }

}
