//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#include "basis/input/Input.hpp"
#include "basis/core/BasisApp.hpp"
#include "basis/core/Window.hpp"
#include "basis/core/WindowSystem.hpp"

namespace basis {
    bool Input::mouse_button_pressed(const int key) {
        auto* window = Application::get().window().native();
        return window::mouse_button_pressed(window, key);
    }

    float Input::mouse_x() {
        return mouse_position().x;
    }

    float Input::mouse_y() {
        return mouse_position().y;
    }

    bool Input::key_pressed(const int key) {
        auto* window = Application::get().window().native();
        return window::key_pressed(window, key);
    }

    bool Input::key_pressed_once(const int key) {
        static std::unordered_map<int, bool> key_states;
        void* window = Application::get().window().native();
        if (!window) return false;

        const bool is_pressed = window::key_pressed(window, key);
        bool& was_pressed_last_frame = key_states[key];

        if (is_pressed && !was_pressed_last_frame) {
            was_pressed_last_frame = true;
            return true;
        }

        if (!is_pressed) {
            was_pressed_last_frame = false;
        }

        return false;
    }

    Vec2 Input::mouse_position() {
        auto* window = Application::get().window().native();
        double x, y;
        window::mouse_position(window, &x, &y);
        return {static_cast<float>(x), static_cast<float>(y)};
    }
}
