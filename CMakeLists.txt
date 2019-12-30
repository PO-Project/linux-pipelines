cmake_minimum_required(VERSION 3.13.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(LinuxPipelines)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

find_library(CURSES_PANEL_LIBRARY NAMES panel DOC "The curses panel library")
if (NOT CURSES_PANEL_LIBRARY)
    message(FATAL_ERROR "Curses' panel library not found.")
endif ()


add_executable(linux-pipelines
    src/main.cpp
)

add_executable(ex
    example/main.cpp
)

add_subdirectory(backends/vim-backend)

target_include_directories(linux-pipelines PRIVATE common-interface ${CURSES_INCLUDE_DIRS} /usr/include/ncurses)
target_link_libraries(linux-pipelines stdc++fs VimBackend ${CURSES_LIBRARIES} ${CURSES_PANEL_LIBRARY})

target_include_directories(ex PRIVATE common-interface src ${CURSES_INCLUDE_DIRS} /usr/include/ncurses)
target_link_libraries(ex stdc++fs ${CURSES_LIBRARIES} ${CURSES_PANEL_LIBRARY})