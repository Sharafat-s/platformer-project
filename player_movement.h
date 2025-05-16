#ifndef PLAYER_MOVEMENT_H
#define PLAYER_MOVEMENT_H

class Player;

namespace PlayerMovement {
    void move_horizontally(Player& player, float delta);
    void update_gravity(Player& player);
    void update(Player& player);
}


/*
class Player;

namespace PlayerMovement {
    void Player::move_player_horizontally(float delta) {
        PlayerMovement::move_horizontally(*this, delta);
    }

    void update_gravity(Player &player);

    void update(Player &player);
};
*/
#endif
