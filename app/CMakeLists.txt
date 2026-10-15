# app/CMakeLists.txt
# ------------------------------------------------
add_executable(BasisApp
               src/main.cpp
               src/App.hpp
)
target_include_directories(BasisApp PRIVATE ${CMAKE_SOURCE_DIR}/app/src)
target_link_libraries(BasisApp PRIVATE Basis)