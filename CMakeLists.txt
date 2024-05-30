cmake_minimum_required(VERSION 3.0)

project(MyRaylibProject)

set(CMAKE_CXX_STANDARD 11)

# Set the path to the raylib include and lib directories
set(RAYLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/raylib/include)
set(RAYLIB_LIB_DIR ${CMAKE_SOURCE_DIR}/thirdparty/raylib/lib)

# Add the include directory
include_directories(${RAYLIB_INCLUDE_DIR})

# Add your source files
add_executable(${PROJECT_NAME} src/main.cpp)

# Link against the raylib library
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIB_DIR}/raylib.lib winmm)

# Copy the raylib DLL to the output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${RAYLIB_LIB_DIR}/raylib.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
