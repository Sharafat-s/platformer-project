#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <string>
#include <vector>

class LevelManager {
public:
    static LevelManager& get_instance();

    static void init(const std::string& filepath);
    static void load_next();
    static void reload();
    static void reset();

    static int get_index();
    static int get_level_count();
    void set_level_count(int count);

    static const std::vector<std::string>& get_raw_level();

private:
    static inline std::string level_file;
    std::vector<std::vector<std::string>> levels;
    int total_level_count = 0;
    int level_index = 0;
};

#endif // LEVEL_MANAGER_H
