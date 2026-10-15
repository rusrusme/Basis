//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include <string>

namespace basis::platform {
    struct FileDialogFilter {
        static constexpr auto ALL = "All Files (*.*)\0*.*\0";
    };

    class FileDialogAPI {
    public:
        static std::string open_file(const char* filter = FileDialogFilter::ALL);
        static std::string save_file(const char* filter = FileDialogFilter::ALL, const char* extension = nullptr);
    };
}
