# basis/CMakeLists.txt
# ------------------------------------------------
add_library(Basis STATIC
            src/core/PCH.hpp
            include/basis/core/BasisTypes.hpp

            src/core/BasisApp.cpp
            include/basis/core/BasisApp.hpp

            include/basis/core/Layer.hpp

            src/core/LayerStack.cpp
            include/basis/core/LayerStack.hpp

            src/core/Timestep.cpp
            include/basis/core/Timestep.hpp

            src/core/Window.cpp
            include/basis/core/Window.hpp

            src/core/WindowSystem.cpp
            include/basis/core/WindowSystem.hpp

            include/basis/event/Event.hpp
            include/basis/event/KeyEvents.hpp
            include/basis/event/MouseEvents.hpp
            include/basis/event/WindowEvents.hpp

            src/core/Log.cpp
            include/basis/core/Log.hpp

            src/config/Config.cpp
            include/basis/config/Config.hpp

            src/input/Input.cpp
            include/basis/input/Input.hpp

            include/basis/input/KeyCodes.hpp
            include/basis/input/MouseCodes.hpp

            include/basis/util/Instrumentor.hpp

            src/platform/FileDialogAPI.cpp
            include/basis/platform/FileDialogAPI.hpp
            src/platform/ShellAPI.cpp
            include/basis/platform/ShellAPI.hpp
            src/platform/win32/Win32FileDialog.cpp
            include/basis/platform/win32/Win32FileDialog.hpp
            src/platform/win32/Win32Shell.cpp
            include/basis/platform/win32/Win32Shell.hpp
)
# ------------------------------------------------
target_include_directories(Basis
                           PUBLIC ${CMAKE_SOURCE_DIR}/basis/include/
                           PRIVATE ${CMAKE_SOURCE_DIR}/basis/src
)
# ------------------------------------------------
if (WIN32)
    target_compile_definitions(Basis PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
    target_link_libraries(Basis PUBLIC comdlg32.lib)
endif ()
# ------------------------------------------------
set(RENDER_API "OPENGL" CACHE STRING "Selects the rendering backend")
set_property(CACHE RENDER_API PROPERTY STRINGS "OPENGL" "VULKAN" "DX12")
if (RENDER_API STREQUAL "OPENGL")
    target_compile_definitions(Basis PUBLIC RENDER_API_OPENGL)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Basis PUBLIC OpenGL::GL GLAD)
elseif (RENDER_API STREQUAL "VULKAN")
    target_compile_definitions(Basis PUBLIC RENDER_API_VULKAN)
elseif (RENDER_API STREQUAL "DX12")
    if (NOT WIN32)
        message(FATAL_ERROR "DirectX 12 is only supported on Windows platforms")
    endif ()
    target_compile_definitions(Basis PUBLIC RENDER_API_DX12)
    target_link_libraries(Basis PUBLIC d3d12.lib dxgi.lib dxguid.lib)
else ()
    message(FATAL_ERROR "Unsupported Render API: ${RENDER_API}")
endif ()
# ------------------------------------------------
target_link_libraries(Basis PUBLIC GLFW GLM)
target_precompile_headers(Basis PRIVATE src/core/PCH.hpp)
# ------------------------------------------------
option(ENABLE_PROFILER "Profiler for performance tracking" ON)
if (ENABLE_PROFILER)
    target_compile_definitions(Basis PUBLIC ENABLE_PROFILER)
endif ()
# ------------------------------------------------