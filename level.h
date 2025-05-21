#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <raylib.h>

struct level {
    int rows;
    int columns;
};

class Level {
public:
    Level();
    ~Level();

    static Level& get_instance();

    [[nodiscard]] char& at(size_t row, size_t column) const {
        return current_level_data[row * current_level.columns + column];
    }

    Level(const Level&) = delete;
    Level(Level&&) = delete;
    Level& operator=(const Level&) = delete;
    Level& operator=(Level&&) = delete;

    [[nodiscard]] int get_rows() const { return current_level.rows; }
    [[nodiscard]] int get_columns() const { return current_level.columns; }

    [[nodiscard]] int get_level_index() const;
    void set_level_index(int index);

    [[nodiscard]] level get_current_level() const;

    char* get_current_level_data();  // Add this

    bool load_all_levels(const std::string& filepath);
    void init_levels(const std::string& filepath);

    void load_level(int offset = 0);
    void unload_level();

    static void reset_level_index();

    //level get_current_level_info();

    static char& get_level_cell(size_t row, size_t column);
    static void set_level_cell(size_t row, size_t column, char chr);

    static bool is_inside_level(int row, int col);
    static bool is_colliding(Vector2 pos, char look_for);

    static char& Level::get_collider(Vector2 pos, char look_for);

    /**/
    void load_level_from_lines(const std::vector<std::string>& lines);

private:
    std::vector<std::vector<std::string>> levels;
    char* current_level_data = nullptr;
    level current_level{};
    int level_index = 0;
};

#endif // LEVEL_H
