cmake_minimum_required(VERSION 3.10)
project(TicTacToeSFML LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source file(s)
add_executable(TicTacToe src/main.cpp) 

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Link SFML
target_link_libraries(TicTacToe PRIVATE sfml-graphics sfml-audio)

# Copy assets folder to build directory on build
add_custom_command(TARGET TicTacToe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:TicTacToe>/assets
)
