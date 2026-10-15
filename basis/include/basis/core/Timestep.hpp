//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

namespace basis {
    class Clock {
    public:
        Clock() { reset(); }

        void reset();
        float update();

    private:
        double last_frame_time_ = 0.0;
    };
}
