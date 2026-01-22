cmake_minimum_required(VERSION 3.20)
project(SpaceInvaders)

set(CMAKE_CXX_STANDARD 23)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

file(GLOB SOURCES "src/*.cpp")

include_directories(src)

add_executable(SpaceInvaders ${SOURCES})

target_link_libraries(SpaceInvaders PRIVATE SFML::Graphics SFML::Window SFML::System)