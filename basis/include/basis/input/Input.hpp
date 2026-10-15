//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include "basis/core/BasisTypes.hpp"

namespace basis {
    class Input {
    public:
        static bool mouse_button_pressed(int key);

        static float mouse_x();
        static float mouse_y();

        static bool key_pressed(int key);
        static bool key_pressed_once(int key);

    private:
        static Vec2 mouse_position();
    };
}
