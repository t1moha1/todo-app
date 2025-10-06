cmake_minimum_required(VERSION 3.15)
project(todo_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(httplib INTERFACE)
target_include_directories(httplib INTERFACE external/httplib)

add_library(json INTERFACE)
target_include_directories(json INTERFACE external/json)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

add_library(db STATIC src/db/DbManager.cpp)
target_include_directories(db PRIVATE src/db
    ${LIBPQXX_INCLUDE_DIRS}
)
target_link_libraries(db PRIVATE ${LIBPQXX_LIBRARIES})

add_library(model STATIC src/model/Task.cpp)
target_include_directories(model PRIVATE src/model)

add_executable(todo-server
    src/main.cpp
)
target_link_libraries(todo-server PRIVATE httplib json db model)
