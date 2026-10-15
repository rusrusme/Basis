//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#include "basis/core/Timestep.hpp"

#include <GLFW/glfw3.h>

#include "basis/core/WindowSystem.hpp"

namespace basis {
    namespace {
        double get_time() {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                return glfwGetTime();
            }
            if (ACTIVE_FRAMEWORK == GraphicsFramework::SDL) {
                return 0.0;
            }
            return 0.0;
        }
    }

    void Clock::reset() {
        last_frame_time_ = get_time();
    }

    float Clock::update() {
        const double current_time = get_time();
        const auto delta_time = static_cast<float>(current_time - last_frame_time_);
        last_frame_time_ = current_time;
        return delta_time;
    }
}
