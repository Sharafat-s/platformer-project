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
                Player::get_instance().reset_player_stats();
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
// int main() {
//     InitWindow(1024, 480, "Platformer");
//     InitAudioDevice();
//     SetTargetFPS(60);
//     // HideCursor(); // Uncomment if needed
//
//     load_fonts();
//     load_images();
//     load_sounds();
//
//     Level::get_instance().init_levels("data/levels.rll"); // or use LevelManager
//
//     game_state = MENU_STATE;
//
//     while (!WindowShouldClose()) {
//         BeginDrawing();
//         ClearBackground(BLACK);
//
//         draw_game();
//         update_game();
//
//         EndDrawing();
//     }
//
//     unload_fonts();
//     unload_images();
//     unload_sounds();
//
//     CloseAudioDevice();
//     CloseWindow();
//
//     return 0;
// }

int main() {
    InitWindow(1024, 480, "Platformer");

    SetTargetFPS(60);
    //HideCursor();

    Level::get_instance().init_levels("data/levels.rll");
    Player::get_instance().reset_player_stats();

    load_fonts();
    load_images();
    load_sounds();

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

// #include "raylib.h"
//
// #include "globals.h"
// #include "level.h"
// #include "player.h"
// #include "enemy.h"
// #include "graphics.h"
// #include "assets.h"
//#include "utilities.h"
// #include "raylib.h"
//
// #include "globals.h"
// #include "player.h"
// #include "enemy.h"
// #include "graphics.h"
// #include "assets.h"
// #include "utilities.h"
// #include "level_manager.h"
//
// void load_test_level();
//
// void update_game() {
//     game_frame++;
//     TraceLog(LOG_INFO, "Current game_state: %d", game_state);
//
//     switch (game_state) {
//         case MENU_STATE:
//             if (IsKeyPressed(KEY_ENTER)) {
//                 SetExitKey(0);
//                 game_state = GAME_STATE;
//                 //LevelManager::init("data/levels.rll");
//                 load_test_level();
//             }
//             break;
//
//         case GAME_STATE:
//             if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
//                 Player::get_instance().move_player_horizontally(PLAYER_MOVEMENT_SPEED);
//             }
//
//             if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
//                 Player::get_instance().move_player_horizontally(-PLAYER_MOVEMENT_SPEED);
//             }
//
//             // Calculating collisions to decide whether the player is allowed to jump
//             Player::get_instance().set_player_on_ground(is_colliding({Player::get_instance().get_player_pos().x, Player::get_instance().get_player_pos().y + 0.1f}, WALL));
//             if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && Player::get_instance().is_player_on_ground()) {
//                 Player::get_instance().set_player_y_velocity(-JUMP_STRENGTH);
//             }
//
//             Player::get_instance().update_player();
//             EnemiesController::get_instance().update_enemies();
//
//             if (IsKeyPressed(KEY_ESCAPE)) {
//                 game_state = PAUSED_STATE;
//             }
//             break;
//
//         case PAUSED_STATE:
//             if (IsKeyPressed(KEY_ESCAPE)) {
//                 game_state = GAME_STATE;
//             }
//             break;
//
//         case DEATH_STATE:
//             Player::get_instance().update_player_gravity();
//
//             if (IsKeyPressed(KEY_ENTER)) {
//                 if (Player::get_instance().get_player_lives() > 0) {
//                     LevelManager::reload();
//                     //load_level(0);
//                     game_state = GAME_STATE;
//                 }
//                 else {
//                     game_state = GAME_OVER_STATE;
//                     PlaySound(game_over_sound);
//                 }
//             }
//             break;
//
//         case GAME_OVER_STATE:
//             if (IsKeyPressed(KEY_ENTER)) {
//                 LevelManager::reset();
//                 //reset_level_index();
//                 Player::get_instance().reset_player_stats();
//                 game_state = GAME_STATE;
//                 //load_level(0);
//             }
//             break;
//
//         case VICTORY_STATE:
//             if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
//                 LevelManager::reset();
//                 //reset_level_index();
//                 Player::get_instance().reset_player_stats();
//                 game_state = MENU_STATE;
//                 SetExitKey(KEY_ESCAPE);
//             }
//             break;
//     }
// }
//
// void draw_game() {
//     switch(game_state) {
//         case MENU_STATE:
//             ClearBackground(BLACK);
//             draw_menu();
//             break;
//
//         case GAME_STATE:
//             ClearBackground(BLACK);
//             draw_parallax_background();
//             draw_level();
//             draw_game_overlay();
//             break;
//
//         case DEATH_STATE:
//             ClearBackground(BLACK);
//             draw_death_screen();
//             break;
//
//         case GAME_OVER_STATE:
//             ClearBackground(BLACK);
//             draw_game_over_menu();
//             break;
//
//         case PAUSED_STATE:
//             ClearBackground(BLACK);
//             draw_pause_menu();
//             break;
//
//         case VICTORY_STATE:
//             draw_victory_menu();
//             break;
//     }
// }
//
// // int main() {
// //     SetConfigFlags(FLAG_VSYNC_HINT);
// //     InitWindow(1024, 480, "Platformer");
// //     SetTargetFPS(60);
// //     HideCursor();
// //
// //     InitAudioDevice();
// //
// //     load_fonts();
// //     load_images();
// //     load_sounds();
// //     //load_level();
// //
// //     while (!WindowShouldClose()) {
// //         BeginDrawing();
// //
// //         update_game();
// //         draw_game();
// //
// //         EndDrawing();
// //     }
// //
// //     SetTraceLogLevel(LOG_ALL);  // shows all logs
// //     while (!IsWindowReady()) {} // wait till it's fully up
// //     while (!WindowShouldClose()) {
// //         BeginDrawing();
// //         ClearBackground(BLACK);
// //         DrawText("Game failed to load assets", 10, 10, 20, RED);
// //         EndDrawing();
// //     }
// //
// //     //unload_level();
// //     unload_sounds();
// //     unload_images();
// //     unload_fonts();
// //
// //     CloseAudioDevice();
// //     CloseWindow();
// //
// //     return 0;
// // }
//
// // Create a minimal test level
// void load_test_level() {
//     const int rows = 12;
//     const int cols = 72;
//
//     // Allocate and initialize level data
//     char* level_data = new char[rows * cols]{
//         '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
//         // (Add the rest of the level manually or copy from your existing array)
//     };
//
//     Level test_level(rows, cols, level_data);
//     delete[] level_data; // safe because data is deep copied in constructor
//     LevelManager::load_hardcoded_level(std::move(test_level));
// }
//
// int main() {
//     SetConfigFlags(FLAG_VSYNC_HINT);
//     InitWindow(1024, 480, "Platformer");
//     SetTargetFPS(60);
//     HideCursor();
//     InitAudioDevice();
//
//     load_fonts();
//     load_images();
//     load_sounds();
//
//     load_test_level(); // <- load hardcoded level here
//
//     while (!WindowShouldClose()) {
//         BeginDrawing();
//         update_game();
//         draw_game();
//         EndDrawing();
//     }
//
//     unload_sounds();
//     unload_images();
//     unload_fonts();
//
//     CloseAudioDevice();
//     if (game_state != GAME_STATE) {
//         while (!WindowShouldClose()) {
//             BeginDrawing();
//             ClearBackground(BLACK);
//             DrawText("Exited unexpectedly. Check logs.", 200, 220, 20, RED);
//             EndDrawing();
//         }
//     }
//
//     CloseWindow();
//     return 0;
// }
