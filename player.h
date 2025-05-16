#ifndef PLAYER_H
#define PLAYER_H

#include <array>
#include "globals.h"
#include "player_movement.h"

class Player {
public:
    static Player& get_instance() {
        static Player instance;
        return instance;
    }

    // Basic stat methods
    void reset_player_stats() {
        lives = MAX_LIVES;
        level_scores.fill(0);
    }

    void increment_score() {
        PlaySound(coin_sound);
        level_scores[level_index]++;
    }

    int get_total_player_score() const {
        int sum = 0;
        for (int score : level_scores)
            sum += score;
        return sum;
    }

    int get_player_lives() const { return lives; }


    // // Movement interface (implemented in PlayerMovement)
    // void move_player_horizontally(float delta);
    // void update_gravity();
    // void update();

    inline void Player::move_player_horizontally(float delta) {
        PlayerMovement::move_horizontally(*this, delta);
    }

    inline void Player::update_player_gravity() {
        PlayerMovement::update_gravity(*this);
    }

    inline void Player::update_player() {
        PlayerMovement::update(*this);
    }

    inline void Player::spawn_player() {
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

    inline void Player::kill_player() {
        PlaySound(player_death_sound);
        game_state = DEATH_STATE;
        lives--;
        level_scores[level_index] = 0;
    }


    Vector2 get_player_pos() const { return position; }
    void set_position(Vector2 pos) { position = pos; }

    float get_player_y_velocity() const { return y_velocity; }
    void set_player_y_velocity(float vel) { y_velocity = vel; }

    bool is_player_on_ground() const { return on_ground; }
    void set_player_on_ground(bool val) { on_ground = val; }

    bool is_looking_forward() const { return looking_forward; }
    void set_looking_forward(bool val) { looking_forward = val; }

    bool is_moving() const { return moving; }
    void set_moving(bool val) { moving = val; }

    int get_time_to_coin_counter() const { return time_to_coin_counter; }
    void increment_time_to_coin_counter(int v) { time_to_coin_counter += v; }
    void reset_time_to_coin_counter() { time_to_coin_counter = 0; }

private:
    Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    float y_velocity = 0;
    Vector2 position = {0, 0};

    bool on_ground = false;
    bool looking_forward = true;
    bool moving = false;

    int lives = MAX_LIVES;
    std::array<int, LEVEL_COUNT> level_scores{};
    int time_to_coin_counter = 0;

    static constexpr int MAX_LIVES = 3;
};
inline Player::Player() {}



#endif

/*
class Player {
public:
    static Player &get_instance () {
        static Player instance;
        return instance;
    }
    static const int MAX_PLAYER_LIVES = 3;

    Player() = default; // private constructor for singleton
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    void reset_player_stats();

    void increment_player_score();

    int get_total_player_score();

    int get_total_player_score() const;

    void spawn_player();

    void kill_player();

    void move_player_horizontally(float delta);

    void update_player_gravity();

    void update_player();

    Vector2 get_player_pos() const { return player_pos; }

    void set_player_pos(const Vector2 &player_pos) {
        this->player_pos = player_pos;
    }

    int get_player_lives() const { return player_lives; }

    [[nodiscard]] float get_player_y_velocity() const {
        return player_y_velocity;
    }

    void set_player_y_velocity(const float player_y_velocity) {
        this->player_y_velocity = player_y_velocity;
    }

    [[nodiscard]] bool is_player_on_ground() const {
        return player_on_ground;
    }

    void set_player_on_ground(const bool is_player_on_ground) {
        this->player_on_ground = is_player_on_ground;
    }

    [[nodiscard]] int get_time_to_coin_counter() const {
        return time_to_coin_counter;
    }

    void set_time_to_coin_counter(const int time_to_coin_counter) {
        this->time_to_coin_counter = time_to_coin_counter;
    }

    void increment_time_to_coin_counter(int value) {
        time_to_coin_counter += value;
    }

    void reset_time_to_coin_counter() {
        time_to_coin_counter = 0;
    }

    [[nodiscard]] bool is_looking_forward() const {
        return looking_forward;
    }

    void set_looking_forward(const bool is_looking_forward) {
        this->looking_forward = is_looking_forward;
    }

    [[nodiscard]] bool is_moving() const {
        return moving;
    }

    void set_moving(const bool is_moving) {
        this->moving = is_moving;
    }

private:
    float player_y_velocity = 0;
    Vector2 player_pos;

    bool player_on_ground;
    bool looking_forward;
    bool moving;

    int player_lives;
    std::array<int, LEVEL_COUNT> player_level_scores;
    int time_to_coin_counter;

};

// === Inline implementations ===

inline void Player::reset_player_stats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

inline void Player::increment_player_score() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

inline int Player::get_total_player_score() {
    int sum = 0;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += player_level_scores[i];
    }
    return sum;
}

inline void Player::spawn_player() {
    player_y_velocity = 0;

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);;

            if (cell == PLAYER) {
                player_pos.x = column;
                player_pos.y = row;
                set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

inline void Player::kill_player() {
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}
*/
/*
void reset_player_stats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

void increment_player_score() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

int get_total_player_score() {
    int sum = 0;

    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += player_level_scores[i];
    }

    return sum;
}

void spawn_player() {
    player_y_velocity = 0;

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);;

            if (cell == PLAYER) {
                player_pos.x = column;
                player_pos.y = row;
                set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

void kill_player() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}

void move_player_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = player_pos.x + delta;
    if (!is_colliding({next_x, player_pos.y}, WALL)) {
        player_pos.x = next_x;
    }
    else {
        player_pos.x = roundf(player_pos.x);
        return;
    }

    // For drawing player animations
    is_looking_forward = delta > 0;
    if (delta != 0) is_moving = true;
}

void update_player_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (is_colliding({player_pos.x, player_pos.y - 0.1f}, WALL) && player_y_velocity < 0) {
        player_y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position
    player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;

    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    is_player_on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
    if (is_player_on_ground) {
        player_y_velocity = 0;
        player_pos.y = roundf(player_pos.y);
    }
}

void update_player() {
    update_player_gravity();

    // Interacting with other level elements
    if (is_colliding(player_pos, COIN)) {
        get_collider(player_pos, COIN) = AIR; // Removes the coin
        increment_player_score();
    }

    if (is_colliding(player_pos, EXIT)) {
        // Reward player for being swift
        if (timer > 0) {
            // For every 9 seconds remaining, award the player 1 coin
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                increment_player_score();
                time_to_coin_counter = 0;
            }
        }
        else {
            // Allow the player to exit after the level timer goes to zero
            load_level(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer >= 0) timer--;
    }

    // Kill the player if they touch a spike or fall below the level
    if (is_colliding(player_pos, SPIKE) || player_pos.y > current_level.rows) {
        kill_player();
    }

    // Upon colliding with an enemy...
    if (EnemiesController::get_instance().is_colliding_with_enemies(player_pos)) {
        // ...check if their velocity is downwards...
        if (player_y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            EnemiesController::get_instance().remove_colliding_enemy(player_pos);
            PlaySound(kill_enemy_sound);

            increment_player_score();
            player_y_velocity = -BOUNCE_OFF_ENEMY;
        }
        else {
            // ...if not, kill the player
            kill_player();
        }
    }
}

#endif
*/