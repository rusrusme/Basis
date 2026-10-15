//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include <functional>

#include "basis/config/Config.hpp"
#include "basis/core/WindowSystem.hpp"

#include "basis/event/WindowEvents.hpp"

namespace basis {
    class Window {
    public:
        using EventCallbackFunc = std::function<void(Event&)>;

        explicit Window(std::string title);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void set_title(const std::string& title);
        void set_event_callback(const EventCallbackFunc& callback);

        void save_config() const;
        void load_config() const;

        void set_vsync(bool enabled);
        [[nodiscard]] bool vsync() const { return config::video::IS_VSYNC; }

        [[nodiscard]] float monitor_dpi_scale() const;
        [[nodiscard]] float aspect_ratio() const { return data_.height > 0 ? static_cast<float>(data_.width) / static_cast<float>(data_.height) : 1.0f; }
        [[nodiscard]] uint32_t width() const { return data_.width; }
        [[nodiscard]] uint32_t height() const { return data_.height; }

        [[nodiscard]] void* native() const { return window_handle_; }
        [[nodiscard]] bool valid() const { return window_handle_ != nullptr; }

    private:
        void update_dimensions();

        WindowProperties data_;
        void* window_handle_ = nullptr;
    };
}
