//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include "basis/core/BasisTypes.hpp"
#include "basis/core/Layer.hpp"

namespace basis {
    class LayerStack {
    public:
        LayerStack() = default;
        ~LayerStack();

        void push_layer(Unique<Layer> layer);
        void push_overlay(Unique<Layer> overlay);

        Unique<Layer> pop_layer(Layer* layer);
        Unique<Layer> pop_overlay(Layer* overlay);

        Vector<Unique<Layer>>::iterator begin() { return layers_.begin(); }
        Vector<Unique<Layer>>::iterator end() { return layers_.end(); }
        [[nodiscard]] Vector<Unique<Layer>>::const_iterator begin() const { return layers_.begin(); }
        [[nodiscard]] Vector<Unique<Layer>>::const_iterator end() const { return layers_.end(); }

        Vector<Unique<Layer>>::reverse_iterator rbegin() { return layers_.rbegin(); }
        Vector<Unique<Layer>>::reverse_iterator rend() { return layers_.rend(); }
        [[nodiscard]] Vector<Unique<Layer>>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
        [[nodiscard]] Vector<Unique<Layer>>::const_reverse_iterator rend() const { return layers_.rend(); }

    private:
        Vector<Unique<Layer>> layers_;
        uint32_t insert_index_ = 0;
    };
}
