cmake_minimum_required(VERSION 3.20)
project(SpaceInvaders)

set(CMAKE_CXX_STANDARD 23)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# --- ВАЖНОЕ ИЗМЕНЕНИЕ ---
# Собираем все .cpp файлы из папки src в переменную SOURCES
file(GLOB SOURCES "src/*.cpp")

# Включаем папку src, чтобы можно было писать #include "Game.hpp" без пути
include_directories(src)

# Создаем исполняемый файл из списка SOURCES
add_executable(SpaceInvaders ${SOURCES})

target_link_libraries(SpaceInvaders PRIVATE SFML::Graphics SFML::Window SFML::System)