#include <iostream>
#include "raylib.h"
#include "globals.h"
#include "player.h"
#include "graphics.h"
#include "assets.h"
#include "level.h"

void update_game() {
    game_frame++;

    switch (game_state) {
        case MENU_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                SetExitKey(0);
                game_state = GAME_STATE;
                Level::get_instance().load_level();
                Player::get_instance().spawn_player();
                EnemiesController::get_instance().spawn_enemies();
            }
            break;

        case GAME_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                Player::get_instance().move_player_horizontally(PLAYER_MOVEMENT_SPEED);
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                Player::get_instance().move_player_horizontally(-PLAYER_MOVEMENT_SPEED);
            }

            Player::get_instance().set_player_on_ground(
                Level::is_colliding({
                                        Player::get_instance().get_player_pos().x,
                                        Player::get_instance().get_player_pos().y + 0.1f
                                    }, WALL)
            );

            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) &&
                Player::get_instance().is_player_on_ground()) {
                Player::get_instance().set_player_y_velocity(-JUMP_STRENGTH);
            }

            Player::get_instance().update_player();
            EnemiesController::get_instance().update_enemies();

            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PAUSED_STATE;
            }
            break;

        case PAUSED_STATE:
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = GAME_STATE;
            }
            break;

        case DEATH_STATE:
            Player::get_instance().update_player_gravity();
            if (IsKeyPressed(KEY_ENTER)) {
                if (Player::get_instance().get_player_lives() > 0) {
                    Level::get_instance().load_level();
                    Player::get_instance().spawn_player();
                    EnemiesController::get_instance().spawn_enemies();
                    /**/
                    LevelManager::reload();
                    game_state = GAME_STATE;
                } else {
                    game_state = GAME_OVER_STATE;
                    PlaySound(game_over_sound);
                }
            }
            break;

        case GAME_OVER_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                Level::reset_level_index();
                LevelManager::get_instance().reset();
                Player::get_instance().reset_player_stats();
                LevelManager::reload();
                game_state = GAME_STATE;
                Level::get_instance().load_level();
                Player::get_instance().spawn_player();
                EnemiesController::get_instance().spawn_enemies();
            }
            break;

        case VICTORY_STATE:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                Level::reset_level_index();
                Player::get_instance().reset_player_stats();
                game_state = MENU_STATE;
                SetExitKey(KEY_ESCAPE);
            }
            break;
    }
}

void draw_game() {
    switch (game_state) {
        case MENU_STATE:
            ClearBackground(BLACK);
            draw_menu();
            break;
        case GAME_STATE:
            ClearBackground(BLACK);
            draw_parallax_background();
            draw_level();
            draw_game_overlay();
            break;
        case DEATH_STATE:
            ClearBackground(BLACK);
            draw_death_screen();
            break;
        case GAME_OVER_STATE:
            ClearBackground(BLACK);
            draw_game_over_menu();
            break;
        case PAUSED_STATE:
            ClearBackground(BLACK);
            draw_pause_menu();
            break;
        case VICTORY_STATE:
            draw_victory_menu();
            break;
        default:
            ClearBackground(RED);
            DrawText("Unknown game state", 100, 100, 20, WHITE);
            break;
    }
}

int main() {
    InitWindow(1024, 480, "Platformer");

    SetTargetFPS(60);
    //HideCursor();

    load_fonts();
    load_images();
    load_sounds();

    Level::get_instance().init_levels("data/levels.rll");
    LevelManager::init("data/levels.rll");
    Player::get_instance().reset_player_stats();

    derive_graphics_metrics_from_loaded_level();

    game_state = MENU_STATE;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        draw_game();
        update_game();

        EndDrawing();
    }

    unload_fonts();
    unload_images();
    unload_sounds();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}