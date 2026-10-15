//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include <functional>

#include "basis/core/BasisTypes.hpp"

namespace basis {
    class Event;

    enum class GraphicsFramework {
        GLFW,
        SDL
    };

    inline auto ACTIVE_FRAMEWORK = GraphicsFramework::GLFW;

    using EventCallbackFun = std::function<void(Event&)>;

    struct WindowProperties {
        String title;
        int width = 0;
        int height = 0;
        EventCallbackFun event_callback = nullptr;
    };

    namespace window {
        bool init(GraphicsFramework framework = GraphicsFramework::GLFW);
        void shutdown();

        void poll_events();
        void swap_buffers(void* handle);
        double time();
        void* proc_address(const char* procname);

        void* create(int width, int height, const char* title, WindowProperties* window_data);
        void destroy(void* handle);

        void make_context_current(void* handle);
        void* current_context();

        void set_position(void* handle, int x, int y);
        void position(void* handle, int* x, int* y);

        void size(void* handle, int* width, int* height);
        void framebuffer_size(void* handle, int* width, int* height);

        void maximize_window(void* handle);
        bool maximized(void* handle);
        bool minimized(void* handle);

        void set_title(void* handle, const char* title);
        void set_vsync(bool enabled);
        float monitor_dpi_scale();

        bool key_pressed(void* handle, int key);
        bool mouse_button_pressed(void* handle, int button);
        void mouse_position(void* handle, double* xpos, double* ypos);

        void setup_callbacks(void* handle, WindowProperties* window_data);
    }
}
