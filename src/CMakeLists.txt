# 'src' CMakeLists.txt

# Add all source and header files in the 'src' folder
file(GLOB_RECURSE SOURCES "*.c" "*.h")

# Create a library target using the source files
add_library(libGB STATIC ${SOURCES})

# Specify the paths to raylib
set(raylib_INCLUDE_DIRS "../packages/raylib-5.0/include")
set(raylib_LIBRARIES "../packages/raylib-5.0/lib/libraylib.a")

# Include raylib headers
target_include_directories(libGB PRIVATE ${raylib_INCLUDE_DIRS})

# Link against raylib
target_link_libraries(libGB PRIVATE ${raylib_LIBRARIES})
