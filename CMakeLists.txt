cmake_minimum_required(VERSION 3.25)
project(PROC_GEN)


include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


# Add your source files here
add_executable(PROC_GEN src/Main.cpp)

# Link other libraries (SFML in this case)
target_link_libraries(PROC_GEN PRIVATE sfml-graphics sfml-system sfml-window)
# Specify the C++ standard
target_compile_features(PROC_GEN PRIVATE cxx_std_17)

# If you need to copy DLLs on Windows (optional)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET PROC_GEN POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:PROC_GEN> $<TARGET_FILE_DIR:PROC_GEN> COMMAND_EXPAND_LISTS)
endif()

# Installation (optional)
install(TARGETS PROC_GEN)
