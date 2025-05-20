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

// #ifndef LEVEL_MANAGER_H
// #define LEVEL_MANAGER_H
//
// #include "level.h"
// //#include <vector>
// #include <string>
// //#include <iostream>
//
// class LevelManager {
// public:
//     static LevelManager& get_instance();
//
//     static void init(const std::string& filepath);
//     static void load_next();
//     static void reload();
//     static void reset();
//
//     static int get_index();
//     std::vector<std::string>& LevelManager::get_raw_level();
//
//     //static std::vector<std::string> get_level();
//
//     int get_level_count() const;
//
//     static std::vector<std::vector<std::string>>& get_levels();
//     void set_levels(const std::vector<std::vector<std::string>>& lvls);
//
//     //void set_levels(const std::vector<level>& lvls);
//
// private:
//     static inline std::string level_file;
//     std::vector<std::vector<std::string>> levels;
//     //std::vector<level> levels;
//     int level_index = 0;
// };
//
// #endif // LEVEL_MANAGER_H

// // level_manager.h
// #ifndef LEVEL_MANAGER_H
// #define LEVEL_MANAGER_H
//
// #include "level.h"
// #include <string>
//
// class LevelManager {
// public:
//     static void init(const std::string& filepath);
//     static void load_next();
//     static void reload();
//     static void reset();
//
//     static int get_index();
//     static LevelManager& get_instance();
//     static Level& get_level();
//
//     int get_level_count() const;
//     void set_level_count(int count);
//
// private:
//     int total_level_count = 0;
//     //static inline std::string level_file;
//     static inline int level_index = 0;
// };
//
// #endif // LEVEL_MANAGER_H

// #ifndef LEVEL_MANAGER_H
// #define LEVEL_MANAGER_H
//
// #include "level.h"
//
// class LevelManager {
// public:
//     // Inside private:
//     static inline std::string level_file;
//
//     static void init(const std::string& filepath);
//     static void load_next();
//     static void reload();
//     static void reset();
//
//     static int get_level_count();
//     static int get_index();
//
//     int get_level_count() const;
//     void set_level_count(int count);
//
//     // Add to level_manager.h
//     static LevelManager& get_instance();
//
//     static Level& get_level();
//
// private:
//     int total_level_count = 0;
//     //static inline std::vector<Level> levels;
//     static inline int level_index = 0;
// };
//
// #endif
