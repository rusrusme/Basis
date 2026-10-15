//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#include "basis/core/LayerStack.hpp"

namespace basis {
    LayerStack::~LayerStack() {
        layers_.reserve(4);
    }

    void LayerStack::push_layer(Unique<Layer> layer) {
        layers_.emplace(layers_.begin() + insert_index_, std::move(layer));
        insert_index_++;
    }

    void LayerStack::push_overlay(Unique<Layer> overlay) {
        layers_.emplace_back(std::move(overlay));
    }

    Unique<Layer> LayerStack::pop_layer(Layer* layer) {
        const auto it = std::find_if(layers_.begin(), layers_.begin() + insert_index_,
                                     [layer](const Unique<Layer>& ptr) { return ptr.get() == layer; });
        if (it != layers_.begin() + insert_index_) {
            (*it)->on_detach();
            Unique<Layer> removed = std::move(*it);
            layers_.erase(it);
            insert_index_--;
            return removed;
        }
        return nullptr;
    }

    Unique<Layer> LayerStack::pop_overlay(Layer* overlay) {
        const auto it = std::find_if(layers_.begin() + insert_index_, layers_.end(),
                                     [overlay](const Unique<Layer>& ptr) { return ptr.get() == overlay; });
        if (it != layers_.end()) {
            (*it)->on_detach();
            Unique<Layer> removed = std::move(*it);
            layers_.erase(it);
            return removed;
        }
        return nullptr;
    }
}
