//
// Copyright (c) 2026 rusrusme
// All rights reserved.
//

#include <utility>

#include "basis/core/BasisApp.hpp"

#include "basis/config/Config.hpp"
#include "basis/core/Log.hpp"
#include "basis/core/Timestep.hpp"
#include "basis/util/Instrumentor.hpp"

namespace basis {
    Application::Application(String title)
        : title_(std::move(title)) {
        instance_ = this;
    }

    void Application::run() {
        PROFILE_FUNCTION();
        Log::init();
        CORE_INFO("Starting application");
        config::load();

        if (!window::init(GraphicsFramework::GLFW)) {
            CORE_CRITICAL("Application initialization failed: Could not initialize window");
            return;
        }
        window_ = std::make_unique<Window>(title_);
        if (!window_->valid()) {
            CORE_CRITICAL("Application initialization failed: Could not create window");
            window::shutdown();
            return;
        }

        window_->set_event_callback([this](Event& event) { on_event(event); });

        CORE_INFO("Attaching application layers");
        for (const auto& layer : layers_) {
            layer->on_attach();
        }
        CORE_INFO("Application startup complete");

        Clock clock;

        // Run core loop
        while (running_) {
            const float delta_time = clock.update();
            // Todo: Frame::begin missing

            ///////////////////////////////////////////////////////
            // Update pass
            PROFILE_START("Application::run - Update");
            for (const auto& layer : layers_) {
                layer->on_update(delta_time);
            }
            PROFILE_END();

            ///////////////////////////////////////////////////////
            // Render pass
            PROFILE_START("Application::run - Render");
            for (const auto& layer : layers_) {
                layer->on_render(delta_time);
            }
            PROFILE_END();
            ///////////////////////////////////////////////////////

            // Todo: Frame::end missing
            window::swap_buffers(window_->native());
            window::poll_events();
        }
        // In reverse
        CORE_INFO("Detaching application layers");
        for (auto it = layers_.rbegin(); it != layers_.rend(); ++it) {
            (*it)->on_detach();
        }

        shutdown();
    }

    void Application::on_event(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowClosedEvent>([this](const WindowClosedEvent& _) {
            CORE_INFO("Window close request received");
            close();
            return true;
        });

        dispatcher.dispatch<WindowResizedEvent>([](const WindowResizedEvent& e) {
            CORE_TRACE("Window resized to {0}x{1}", e.width(), e.height());
            return false; // Forward to layer
        });

        // From top to bottom
        for (auto it = layers_.rbegin(); it != layers_.rend(); ++it) {
            if (event.handled) {
                break; // Event consumed
            }
            (*it)->on_event(event);
        }
    }

    void Application::push_layer(Unique<Layer> layer) {
        layers_.push_layer(std::move(layer));
    }

    void Application::push_overlay(Unique<Layer> overlay) {
        layers_.push_overlay(std::move(overlay));
    }

    void Application::shutdown() {
        CORE_INFO("Shutting down application application");

        window_.reset();

        config::save();
        window::shutdown();
        CORE_INFO("Application shutdown complete");
    }

    Application* Application::instance_ = nullptr;
}
