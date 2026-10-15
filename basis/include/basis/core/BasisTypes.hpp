//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace basis {
    using String = std::string;

    template<typename T>
    using Shared = std::shared_ptr<T>;

    template<typename T>
    using Unique = std::unique_ptr<T>;

    template<typename T>
    using Vector = std::vector<T>;

    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;
}
