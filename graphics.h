#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include "enemies_controller.h"
#include "globals.h"
#include "Player.h"
#include "level.h"

inline void draw_text(Text &text) {
    if (!text.font) {
        TraceLog(LOG_ERROR, "Font is null in draw_text");
        return;
    }

    // Measure the text, center it to the required position, and draw it
    Vector2 dimensions = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);

    Vector2 pos = {
        (screen_size.x * text.position.x) - (0.5f * dimensions.x),
        (screen_size.y * text.position.y) - (0.5f * dimensions.y)
    };

    DrawTextEx(*text.font, text.str.c_str(), pos, dimensions.y, text.spacing, text.color);
}

inline void derive_graphics_metrics_from_loaded_level() {
    // Level and UI setup
    screen_size.x = static_cast<float>(GetScreenWidth());
    screen_size.y = static_cast<float>(GetScreenHeight());

    cell_size = screen_size.y / static_cast<float>(Level::get_instance().get_current_level().rows);
    screen_scale = std::min(screen_size.x, screen_size.y) / SCREEN_SCALE_DIVISOR;

    // Parallax background setup
    float larger_screen_side = std::max(screen_size.x, screen_size.y);

    if (screen_size.x > screen_size.y) {
        background_size = {larger_screen_side, larger_screen_side / 16 * 10};
    } else {
        background_size = {larger_screen_side / 10 * 16, larger_screen_side};
    }

    background_y_offset = (screen_size.y - background_size.y) * 0.5f;
}

inline void draw_parallax_background() {
    // First uses the player's position
    float initial_offset = -(Player::get_instance().get_player_pos().x * PARALLAX_PLAYER_SCROLLING_SPEED + game_frame *
                             PARALLAX_IDLE_SCROLLING_SPEED);

    // Calculate offsets for different layers
    float background_offset = initial_offset;
    float middleground_offset = background_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE;
    float foreground_offset = middleground_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE;

    // Wrap offsets to create a loop effect
    background_offset = fmod(background_offset, 1.0f);
    middleground_offset = fmod(middleground_offset, 1.0f);
    foreground_offset = fmod(foreground_offset, 1.0f);

    // Scale to background size
    background_offset *= background_size.x;
    middleground_offset *= background_size.x;
    foreground_offset *= background_size.x;

    // Each layer is drawn twice, side by side, the first starting from its offset, and the other from its offset + background_size
    // This ensures a seamless scrolling effect, because when one copy moves out of sight, the second jumps into its place.
    draw_image(background, {background_offset + background_size.x, background_y_offset}, background_size.x,
               background_size.y);
    draw_image(background, {background_offset, background_y_offset}, background_size.x, background_size.y);

    draw_image(middleground, {middleground_offset + background_size.x, background_y_offset}, background_size.x,
               background_size.y);
    draw_image(middleground, {middleground_offset, background_y_offset}, background_size.x, background_size.y);

    draw_image(foreground, {foreground_offset + background_size.x, background_y_offset}, background_size.x,
               background_size.y);
    draw_image(foreground, {foreground_offset, background_y_offset}, background_size.x, background_size.y);
}

inline void draw_image(const Texture2D &image, Vector2 pos, float width, float height) {
    DrawTexturePro(
        image,
        {0, 0, (float) image.width, (float) image.height},
        {pos.x, pos.y, width, height},
        {0, 0},
        0.0f,
        WHITE
    );
}

inline void draw_game_overlay() {
    const float ICON_SIZE = 48.0f * screen_scale;

    float slight_vertical_offset = 8.0f;
    slight_vertical_offset *= screen_scale;

    // Hearts
    for (int i = 0; i < Player::get_instance().get_player_lives(); i++) {
        const float SPACE_BETWEEN_HEARTS = 4.0f * screen_scale;
        draw_image(heart_image, {ICON_SIZE * i + SPACE_BETWEEN_HEARTS, slight_vertical_offset}, ICON_SIZE);
    }

    // Timer
    Vector2 timer_dimensions = MeasureTextEx(menu_font, std::to_string(timer / 60).c_str(), ICON_SIZE, 2.0f);
    Vector2 timer_position = {(GetRenderWidth() - timer_dimensions.x) * 0.5f, slight_vertical_offset};
    DrawTextEx(menu_font, std::to_string(timer / 60).c_str(), timer_position, ICON_SIZE, 2.0f, WHITE);

    // Score
    Vector2 score_dimensions = MeasureTextEx(
        menu_font, std::to_string(Player::get_instance().get_total_player_score()).c_str(), ICON_SIZE,
        2.0f);
    Vector2 score_position = {GetRenderWidth() - score_dimensions.x - ICON_SIZE, slight_vertical_offset};
    DrawTextEx(menu_font, std::to_string(Player::get_instance().get_total_player_score()).c_str(), score_position,
               ICON_SIZE, 2.0f, WHITE);
    draw_sprite(coin_sprite, {GetRenderWidth() - ICON_SIZE, slight_vertical_offset}, ICON_SIZE);
}

inline void draw_level() {
    // Move the x-axis' center to the middle of the screen
    horizontal_shift = (screen_size.x - cell_size) / 2;

    Level &current_level = Level::get_instance();

    if (!current_level.get_current_level_data()) {
        TraceLog(LOG_FATAL, "Level data is null before draw_level()");
        exit(EXIT_FAILURE);
    }

    int rows = current_level.get_current_level().rows;
    int columns = current_level.get_current_level().columns;

    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            Vector2 pos = {
                (static_cast<float>(column) - Player::get_instance().get_player_pos().x) * cell_size + horizontal_shift,
                static_cast<float>(row) * cell_size
            };

            char cell = current_level.get_level_cell(row, column);
            switch (cell) {
                case WALL:
                    draw_image(wall_image, pos, cell_size);
                    break;
                case WALL_DARK:
                    draw_image(wall_dark_image, pos, cell_size);
                    break;
                case SPIKE:
                    draw_image(spike_image, pos, cell_size);
                    break;
                case COIN:
                    draw_sprite(coin_sprite, pos, cell_size);
                    break;
                case EXIT:
                    draw_image(exit_image, pos, cell_size);
                    break;
                default:
                    break;
            }
        }
    }
    draw_player();
    draw_enemies();
}

inline void draw_image(Texture2D &image, Vector2 pos, float size) {
    draw_image(image, pos, size, size);
}

inline void draw_player() {
    horizontal_shift = (screen_size.x - cell_size) / 2;

    // Shift the camera to the center of the screen to allow to see what is in front of the player
    Vector2 pos = {
        horizontal_shift,
        Player::get_instance().get_player_pos().y * cell_size
    };

    // Pick an appropriate sprite for the player
    if (game_state == GAME_STATE) {
        if (!Player::get_instance().is_player_on_ground()) {
            draw_image((Player::get_instance().is_looking_forward()
                            ? player_jump_forward_image
                            : player_jump_backwards_image), pos, cell_size);
        } else if (Player::get_instance().is_moving()) {
            draw_sprite((Player::get_instance().is_looking_forward()
                             ? player_walk_forward_sprite
                             : player_walk_backwards_sprite), pos,
                        cell_size);
            Player::get_instance().set_moving(false);
        } else {
            draw_image((Player::get_instance().is_looking_forward()
                            ? player_stand_forward_image
                            : player_stand_backwards_image), pos,
                       cell_size);
        }
    } else {
        draw_image(player_dead_image, pos, cell_size);
    }
}

inline void draw_enemies() {
    // Go over all enemies and draw them, once again accounting to the player's movement and horizontal shift
    for (auto &enemy: EnemiesController::get_instance().get_enemies()) {
        horizontal_shift = (screen_size.x - cell_size) / 2;

        Vector2 pos = {
            (enemy.get_pos().x - Player::get_instance().get_player_pos().x) * cell_size + horizontal_shift,
            enemy.get_pos().y * cell_size
        };

        draw_sprite(enemy_walk, pos, cell_size);
    }
}

// Menus
inline void draw_menu() {
    draw_text(game_title);
    draw_text(game_subtitle);
}

inline void draw_pause_menu() {
    draw_text(game_paused);
}

inline void draw_death_screen() {
    draw_parallax_background();
    draw_level();
    draw_game_overlay();
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), {0, 0, 0, 100});
    draw_text(death_title);
    draw_text(death_subtitle);
}

inline void draw_game_over_menu() {
    draw_text(game_over_title);
    draw_text(game_over_subtitle);
}

inline void create_victory_menu_background() {
    for (auto &ball: victory_balls) {
        ball.x = rand_up_to(screen_size.x);
        ball.y = rand_up_to(screen_size.y);
        ball.dx = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dx *= screen_scale;
        if (abs(ball.dx) < 0E-1) ball.dx = 1.0f;
        ball.dy = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dy *= screen_scale;
        if (abs(ball.dy) < 0E-1) ball.dy = 1.0f;
        ball.radius = rand_from_to(VICTORY_BALL_MIN_RADIUS, VICTORY_BALL_MAX_RADIUS);
        ball.radius *= screen_scale;
    }
}

inline void animate_victory_menu_background() {
    for (auto &ball: victory_balls) {
        ball.x += ball.dx;
        if (ball.x - ball.radius < 0 ||
            ball.x + ball.radius >= screen_size.x) {
            ball.dx = -ball.dx;
        }
        ball.y += ball.dy;
        if (ball.y - ball.radius < 0 ||
            ball.y + ball.radius >= screen_size.y) {
            ball.dy = -ball.dy;
        }
    }
}

inline void draw_victory_menu_background() {
    for (auto &ball: victory_balls) {
        DrawCircleV({ball.x, ball.y}, ball.radius, VICTORY_BALL_COLOR);
    }
}

inline void draw_victory_menu() {
    DrawRectangle(
        0, 0,
        static_cast<int>(screen_size.x), static_cast<int>(screen_size.y),
        {0, 0, 0, VICTORY_BALL_TRAIL_TRANSPARENCY}
    );

    animate_victory_menu_background();
    draw_victory_menu_background();

    draw_text(victory_title);
    draw_text(victory_subtitle);
}

inline float rand_from_to(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

inline float rand_up_to(float max) {
    return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / max));
}

#endif //GRAPHICS_H
