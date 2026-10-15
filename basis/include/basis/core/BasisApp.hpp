//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include "basis/config/Config.hpp"
#include "basis/core/BasisTypes.hpp"
#include "basis/core/LayerStack.hpp"
#include "basis/core/Window.hpp"

namespace basis {
    class Event;

    class Application {
    public:
        explicit Application(String title = config::APP_TITLE);
        virtual ~Application() = default;

        void run();
        void close() { running_ = false; }

        void on_event(Event& event);

        [[nodiscard]] Window& window() const { return *window_; }
        static Application& get() { return *instance_; }

    protected:
        void push_layer(Unique<Layer> layer);
        void push_overlay(Unique<Layer> overlay);

    private:
        void shutdown();

        Unique<Window> window_;
        LayerStack layers_;

        String title_;
        bool running_ = true;

        static Application* instance_;
    };
}
