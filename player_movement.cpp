#include "player_movement.h"
#include "Player.h"
#include "enemies_controller.h"
#include <cmath>

void PlayerMovement::move_horizontally(Player& player, float delta) {
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

void PlayerMovement::update_gravity(Player& player) {
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

void PlayerMovement::update(Player& player) {
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

/*
void PlayerMovement::move_horizontally(Player& player, float delta) {
    Vector2 player_pos = player.get_player_pos();
    float next_x = player_pos.x + delta;
    if (!is_colliding({next_x, player.get_player_pos().y}, WALL)) {
        player_pos.x = next_x;
        player.set_player_pos(player_pos);
    } else {
        player_pos.x = roundf(player_pos.x);
        player.set_player_pos(player_pos);
        return;
    }

    player.set_looking_forward(delta > 0);
    if (delta != 0) {
        player.set_moving(true);
    }

}

void PlayerMovement::update_gravity(Player& player) {
    Vector2 player_pos = player.get_player_pos();
    float velocity = player.get_player_y_velocity();

    if (is_colliding({player_pos.x, player_pos.y - 0.1f}, WALL) && velocity < 0) {
        velocity = CEILING_BOUNCE_OFF;
    }

    bool on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
    if (on_ground) {
        velocity = 0;
        player_pos.y = roundf(player_pos.y);
    }

    // Update player state
    player.set_player_pos(player_pos);
    player.set_player_y_velocity(velocity);
    player.set_player_on_ground(on_ground);
}

void PlayerMovement::update(Player& player) {
    update_gravity(player);

    if (is_colliding(player.get_player_pos(), COIN)) {
        get_collider(player.get_player_pos(), COIN) = AIR;
        player.increment_player_score();
    }

    if (is_colliding(player.get_player_pos(), EXIT)) {
        if (timer > 0) {
            timer -= 25;

            player.increment_time_to_coin_counter(5);
            if (player.get_time_to_coin_counter() / 60 > 1) {
                player.increment_player_score();
                player.reset_time_to_coin_counter();
            }
        } else {
            load_level(1);
            PlaySound(exit_sound);
        }
    } else {
        if (timer >= 0) timer--;
    }

    if (is_colliding(player.get_player_pos(), SPIKE) || player.get_player_pos().y > current_level.rows) {
        player.kill_player();
    }

    if (EnemiesController::get_instance().is_colliding_with_enemies(player.get_player_pos())) {
        if (player.get_player_y_velocity() > 0) {
            EnemiesController::get_instance().remove_colliding_enemy(player.get_player_pos());
            PlaySound(kill_enemy_sound);
            player.increment_player_score();
            player.set_player_y_velocity(-BOUNCE_OFF_ENEMY);
        } else {
            player.kill_player();
        }
    }
}
*/