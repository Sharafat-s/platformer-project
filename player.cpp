// #include "Player.h"
// #include <cmath>
//
// Player& Player::get_instance() {
//     static Player instance;
//     return instance;
// }
//
// Player::Player()
//     : y_velocity(0),
//       position({0, 0}),
//       on_ground(false),
//       looking_forward(true),
//       moving(false),
//       lives(MAX_LIVES),
//       level_scores{},
//       time_to_coin_counter(0)
// {}
//
// void Player::reset_stats() {
//     lives = MAX_LIVES;
//     level_scores.fill(0);
// }
//
// void Player::increment_score() {
//     PlaySound(coin_sound);
//     level_scores[level_index]++;
// }
//
// int Player::get_total_score() const {
//     int sum = 0;
//     for (int score : level_scores) {
//         sum += score;
//     }
//     return sum;
// }
//
// void Player::spawn() {
//     y_velocity = 0;
//     for (size_t row = 0; row < current_level.rows; ++row) {
//         for (size_t column = 0; column < current_level.columns; ++column) {
//             if (get_level_cell(row, column) == PLAYER) {
//                 position.x = column;
//                 position.y = row;
//                 set_level_cell(row, column, AIR);
//                 return;
//             }
//         }
//     }
// }
//
// void Player::kill() {
//     PlaySound(player_death_sound);
//     game_state = DEATH_STATE;
//     lives--;
//     level_scores[level_index] = 0;
// }

