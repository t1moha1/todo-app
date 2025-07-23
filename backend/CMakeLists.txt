cmake_minimum_required(VERSION 3.15)
project(todo_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к header-only библиотекам
include_directories(
    ${PROJECT_SOURCE_DIR}/external/httplib
    ${PROJECT_SOURCE_DIR}/external/json
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

add_executable(todo-server
    src/main.cpp
    src/DbManager.cpp
    src/model/Task.cpp
)

target_link_libraries(todo-server
    ${LIBPQXX_LIBRARIES}
)

target_include_directories(todo-server PRIVATE
    ${LIBPQXX_INCLUDE_DIRS}
)