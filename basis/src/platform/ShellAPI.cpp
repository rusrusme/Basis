//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#include "basis/platform/ShellAPI.hpp"

#include "basis/core/BasisTypes.hpp"

#if defined(_WIN32)
#include "basis/platform/win32/Win32Shell.hpp"
#elif defined(__linux__)
#include "basis/platform/linux/LinuxShell.hpp"
#endif

namespace basis::platform {
    void ShellAPI::open_url(const String& url) {
        #if defined(_WIN32)
        return win32::open_url(url);
        #elif defined(__linux__)
        // return linux::open_file(filter);
        return "";
        #else
        return "";
        #endif
    }
}
