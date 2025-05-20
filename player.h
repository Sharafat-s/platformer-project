#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "globals.h"
#include "level_manager.h"

class EnemiesController;

class Player {
public:
    static Player& get_instance();

    // Basic stat methods
    void reset_player_stats();
    void increment_score();
    int get_total_player_score() const;
    int get_player_lives() const;

    // Movement interface
    void move_player_horizontally(float delta);
    void update_player_gravity();
    void update_player();
    void spawn_player();
    void kill_player();

    // Accessors and mutators
    Vector2 get_player_pos() const;
    void set_position(Vector2 pos);

    float get_player_y_velocity() const;
    void set_player_y_velocity(float vel);

    bool is_player_on_ground() const;
    void set_player_on_ground(bool val);

    bool is_looking_forward() const;
    void set_looking_forward(bool val);

    bool is_moving() const;
    void set_moving(bool val);

    int get_time_to_coin_counter() const;
    void increment_time_to_coin_counter(int v);
    void reset_time_to_coin_counter();

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
    std::vector<int> level_scores;
    //std::array<int, LEVEL_COUNT> level_scores{};
    int time_to_coin_counter = 0;

    static constexpr int MAX_LIVES = 3;
};

// PlayerMovement Logic
namespace PlayerMovement {
    void move_horizontally(Player& player, float delta);
    void update_gravity(Player& player);
    void update(Player& player);
}

#endif // PLAYER_H