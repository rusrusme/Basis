//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#include "basis/config/Config.hpp"

#include <fstream>
#include <sstream>

#include "basis/core/Log.hpp"

namespace basis {
    void config::load() {
        std::ifstream file(CONFIG_FILE_PATH);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#' || line[0] == ';') continue;

            std::istringstream iss(line);
            std::string key;
            if (!(iss >> key)) continue;

            if (key == "is_vsync") {
                int int_val;
                if (iss >> int_val) {
                    video::IS_VSYNC = (int_val != 0);
                }
                else {
                    iss.clear();
                    std::string str_val;
                    if (iss >> str_val) {
                        video::IS_VSYNC = (str_val == "true" || str_val == "1");
                    }
                }
            }
            else if (key == "window_width") {
                iss >> video::WINDOW_WIDTH;
            }
            else if (key == "window_height") {
                iss >> video::WINDOW_HEIGHT;
            }
            else if (key == "window_x") {
                iss >> video::WINDOW_X;
            }
            else if (key == "window_y") {
                iss >> video::WINDOW_Y;
            }
            else if (key == "is_maximized") {
                int val;
                if (iss >> val) video::IS_MAXIMIZED = (val != 0);
            }
        }
        CORE_TRACE("Configuration settings loaded");
    }

    void config::save() {
        std::ofstream file(CONFIG_FILE_PATH);
        if (!file.is_open()) {
            CORE_ERROR("Failed to open '{0}' for writing config", CONFIG_FILE_PATH);
            return;
        }
        file << "is_vsync " << (video::IS_VSYNC ? 1 : 0) << "\n";
        file << "window_width " << video::WINDOW_WIDTH << "\n";
        file << "window_height " << video::WINDOW_HEIGHT << "\n";
        file << "window_x " << video::WINDOW_X << "\n";
        file << "window_y " << video::WINDOW_Y << "\n";
        file << "is_maximized " << (video::IS_MAXIMIZED ? 1 : 0) << "\n";

        CORE_TRACE("Configuration saved");
    }
}
