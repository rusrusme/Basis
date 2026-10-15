//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#include "basis/core/WindowSystem.hpp"

#include <GLFW/glfw3.h>

#include "basis/core/Log.hpp"
#include "basis/event/KeyEvents.hpp"
#include "basis/event/MouseEvents.hpp"
#include "basis/event/WindowEvents.hpp"

namespace basis::window {
    static bool initialized = false;

    static int get_glfw_key(const int engine_key) {
        return engine_key;
    }

    static int get_core_key(const int glfw_key) {
        return glfw_key;
    }

    static int get_glfw_mouse_button(const int engine_button) {
        return engine_button;
    }

    static int get_core_mouse_button(const int glfw_button) {
        return glfw_button;
    }

    bool init(const GraphicsFramework framework) {
        ACTIVE_FRAMEWORK = framework;
        switch (ACTIVE_FRAMEWORK) {
            case GraphicsFramework::GLFW :
                if (!glfwInit()) {
                    CORE_CRITICAL("Failed to initialize GLFW");
                    return false;
                }
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                break;
            case GraphicsFramework::SDL :
                return false;
                break;
        }
        initialized = true;
        return true;
    }

    void shutdown() {
        if (!initialized) {
            CORE_ERROR("Tried to call basis::window::shutdown before initializing");
            return;
        }
        switch (ACTIVE_FRAMEWORK) {
            case GraphicsFramework::GLFW :
                glfwTerminate();
                break;
            case GraphicsFramework::SDL :
                break;
        }
        initialized = false;
    }

    void poll_events() {
        if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
            glfwPollEvents();
        }
        else {
            CORE_WARN("No valid graphics framework found");
        }
    }

    void swap_buffers(void* handle) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwSwapBuffers(static_cast<GLFWwindow*>(handle));
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::swap_buffers: {0}", handle);
        }
    }

    double time() {
        if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
            return glfwGetTime();
        }
        CORE_WARN("No valid graphics framework found");
        return 0.0;
    }

    void* proc_address(const char* procname) {
        if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
            return reinterpret_cast<void*>(glfwGetProcAddress(procname));
        }
        CORE_WARN("No valid graphics framework found");
        return nullptr;
    }

    void* create(int width, int height, const char* title, WindowProperties* window_data) {
        if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
            #if defined(DEBUG) || defined(_DEBUG)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
            #endif

            GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
            if (window && window_data) {
                setup_callbacks(window, window_data);
            }
            return window;
        }
        else {
            CORE_WARN("No valid graphics framework found");
        }
        return nullptr;
    }

    void destroy(void* handle) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwDestroyWindow(static_cast<GLFWwindow*>(handle));
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::destroy_window: {0}", handle);
        }
    }

    void make_context_current(void* handle) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwMakeContextCurrent(static_cast<GLFWwindow*>(handle));
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::make_context_current: {0}", handle);
        }
    }

    void* current_context() {
        if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
            return glfwGetCurrentContext();
        }
        CORE_WARN("No valid graphics framework found");
        return nullptr;
    }

    void setup_callbacks(void* handle, WindowProperties* window_data) {
        if (!handle || !window_data || ACTIVE_FRAMEWORK != GraphicsFramework::GLFW) return;

        auto* window = static_cast<GLFWwindow*>(handle);
        glfwSetWindowUserPointer(window, window_data);

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, const int width, const int height) {
            auto* data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));
            if (data && data->event_callback) {
                data->width = width;
                data->height = height;
                WindowResizedEvent event(width, height);
                data->event_callback(event);
            }
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* win) {
            auto* data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));
            if (data && data->event_callback) {
                WindowClosedEvent event;
                data->event_callback(event);
            }
        });

        glfwSetKeyCallback(window, [](GLFWwindow* win, const int key, int scancode, int action, int mods) {
            auto* data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));
            if (!data || !data->event_callback) return;
            const int engine_key = get_core_key(key);

            switch (action) {
                case GLFW_PRESS : {
                    KeyPressedEvent event(engine_key, false);
                    data->event_callback(event);
                    break;
                }
                case GLFW_RELEASE : {
                    KeyReleasedEvent event(engine_key);
                    data->event_callback(event);
                    break;
                }
                case GLFW_REPEAT : {
                    KeyPressedEvent event(engine_key, true);
                    data->event_callback(event);
                    break;
                }
                default : break;
            }
        });

        glfwSetCharCallback(window, [](GLFWwindow* win, unsigned int keycode) {
            auto* data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));
            if (!data || !data->event_callback) return;
            KeyTypedEvent event(static_cast<int>(keycode));
            data->event_callback(event);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* win, const int button, int action, int mods) {
            auto* data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));
            if (!data || !data->event_callback) return;
            const int engine_button = get_core_mouse_button(button);

            if (action == GLFW_PRESS) {
                MouseButtonPressedEvent event(engine_button);
                data->event_callback(event);
            }
            else if (action == GLFW_RELEASE) {
                MouseButtonReleasedEvent event(engine_button);
                data->event_callback(event);
            }
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
            auto* data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));
            if (!data || !data->event_callback) return;
            MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
            data->event_callback(event);
        });

        glfwSetScrollCallback(window, [](GLFWwindow* win, const double x_offset, const double y_offset) {
            auto* data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));
            if (data && data->event_callback) {
                MouseScrolledEvent event(static_cast<float>(x_offset), static_cast<float>(y_offset));
                data->event_callback(event);
            }
        });
    }

    void set_position(void* handle, int x, int y) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwSetWindowPos(static_cast<GLFWwindow*>(handle), x, y);
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::set_position: {0}", handle);
        }
    }

    void position(void* handle, int* x, int* y) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwGetWindowPos(static_cast<GLFWwindow*>(handle), x, y);
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::get_position: {0}", handle);
        }
    }

    void size(void* handle, int* width, int* height) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwGetWindowSize(static_cast<GLFWwindow*>(handle), width, height);
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::get_size: {0}", handle);
        }
    }

    void framebuffer_size(void* handle, int* width, int* height) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwGetFramebufferSize(static_cast<GLFWwindow*>(handle), width, height);
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::get_framebuffer_size: {0}", handle);
        }
    }

    void maximize_window(void* handle) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwMaximizeWindow(static_cast<GLFWwindow*>(handle));
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::maximize_window: {0}", handle);
        }
    }

    bool maximized(void* handle) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                return glfwGetWindowAttrib(static_cast<GLFWwindow*>(handle), GLFW_MAXIMIZED) != 0;
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::is_maximized: {0}", handle);
        }
        return false;
    }

    bool minimized(void* handle) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                return glfwGetWindowAttrib(static_cast<GLFWwindow*>(handle), GLFW_ICONIFIED) != 0;
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::is_minimized: {0}", handle);
        }
        return false;
    }

    void set_title(void* handle, const char* title) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwSetWindowTitle(static_cast<GLFWwindow*>(handle), title);
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::set_title: {0}", handle);
        }
    }

    void set_vsync(bool enabled) {
        if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
            glfwSwapInterval(enabled ? 1 : 0);
        }
        else {
            CORE_WARN("No valid graphics framework found");
        }
    }

    float monitor_dpi_scale() {
        if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            if (!monitor) return 1.0f;
            float xscale, yscale;
            glfwGetMonitorContentScale(monitor, &xscale, &yscale);
            return xscale;
        }
        CORE_WARN("No valid graphics framework found");
        return 1.0f;
    }

    bool key_pressed(void* handle, const int key) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                const int state = glfwGetKey(static_cast<GLFWwindow*>(handle), get_glfw_key(key));
                return state == GLFW_PRESS || state == GLFW_REPEAT;
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::is_key_pressed: {0}", handle);
        }
        return false;
    }

    bool mouse_button_pressed(void* handle, const int button) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                const int state = glfwGetMouseButton(static_cast<GLFWwindow*>(handle), get_glfw_mouse_button(button));
                return state == GLFW_PRESS;
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::is_mouse_button_pressed: {0}", handle);
        }
        return false;
    }

    void mouse_position(void* handle, double* xpos, double* ypos) {
        if (handle) {
            if (ACTIVE_FRAMEWORK == GraphicsFramework::GLFW) {
                glfwGetCursorPos(static_cast<GLFWwindow*>(handle), xpos, ypos);
            }
        }
        else {
            CORE_ERROR("Invalid window handle in basis::window::get_mouse_position: {0}", handle);
            if (xpos) *xpos = 0.0;
            if (ypos) *ypos = 0.0;
        }
    }
}
