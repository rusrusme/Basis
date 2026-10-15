//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include "basis/core/BasisApp.hpp"

class App : public basis::Application {
public:
    App() : Application("Test App") {}
    ~App() override = default;
};
