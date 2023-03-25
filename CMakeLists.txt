cmake_minimum_required(VERSION 3.0.0)

project(Elden-CLIng)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set (include_dir "${PROJECT_SOURCE_DIR}/include/")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files "${source_dir}/*.cpp" "${source_dir}/*.c")
file (GLOB header_files "${include_dir}/*.h" "${include_dir}/*.hpp")

include_directories(${include_dir})

#find_package(ncursescpp REQUIRED)
find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${source_files} ${header_files})

target_link_libraries(${PROJECT_NAME} PUBLIC ${CURSES_LIBRARIES} menu ncurses)
