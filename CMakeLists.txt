cmake_minimum_required(VERSION 3.20)

project(gtk_proj)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk4 REQUIRED gtk4)
include_directories(${gtk4_INCLUDE_DIRS})

add_executable(gtk_proj main.c)
target_link_libraries(gtk_proj ${gtk4_LIBRARIES})

target_include_directories(gtk_proj PRIVATE ${GTK4_INCLUDE_DIRS})
