//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

namespace basis::config {
    constexpr auto APP_TITLE = "Application";

    namespace video {
        constexpr int MONITOR_RESOLUTION_WIDTH = 1920;
        constexpr int MONITOR_RESOLUTION_HEIGHT = 1080;

        inline bool IS_VSYNC = false;

        inline int WINDOW_WIDTH = 1600;
        inline int WINDOW_HEIGHT = 900;
        inline int WINDOW_X = -1; // -1 indicates default OS centered placement
        inline int WINDOW_Y = -1;
        inline bool IS_MAXIMIZED = false;
    }

    constexpr auto CONFIG_FILE_PATH = "config.ini";

    void load();
    void save();
}
