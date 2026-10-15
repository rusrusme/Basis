include(FetchContent)
# ------------------------------------------------
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.5.1
        GIT_SHALLOW TRUE
        GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(glfw)
# ------------------------------------------------
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
        GIT_SHALLOW TRUE
        GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(glm)
# ------------------------------------------------
add_library(glad STATIC thirdparty/glad/src/glad.c)
target_include_directories(glad PUBLIC thirdparty/glad/include)
# ------------------------------------------------
add_library(GLFW ALIAS glfw)
add_library(GLM ALIAS glm)
add_library(GLAD ALIAS glad)
# ------------------------------------------------