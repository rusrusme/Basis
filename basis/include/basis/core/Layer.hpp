//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include "basis/core/BasisTypes.hpp"

namespace basis {
    class Event;

    class Layer {
    public:
        explicit Layer(String title = "Layer") : title_(std::move(title)) {}
        virtual ~Layer() = default;

        virtual void on_attach() {}
        virtual void on_detach() {}

        virtual void on_update(float delta_time) {}
        virtual void on_render(float delta_time) {}

        virtual void on_event(Event& event) {}

    protected:
        String title_;
    };
}
