cmake_minimum_required(VERSION 3.12)
project(GameBoy)

# Set C++ standard (optional)
set(CMAKE_CXX_STANDARD 11)

# Add the 'src' folder as a source directory
add_subdirectory(src)


# Your executable or library target
add_executable(GameBoy.exe src/main.c)

# Link against the libraries from the 'packages' folder
target_link_libraries(GameBoy.exe PRIVATE libGB)

target_include_directories(GameBoy.exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(GameBoy.exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/packages/raylib-5.0/include)

