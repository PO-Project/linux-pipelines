cmake_minimum_required(VERSION 3.10)
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


add_executable(vim-linux-pipelines
    src/vim-main.cpp
)

add_executable(example-linux-pipelines
    example/main.cpp
)

add_executable(nice-linux-pipelines
    src/nice-main.cpp
)

add_subdirectory(backends/vim-backend)

include_directories(common-interface)
add_subdirectory(backends/backend-nice)

target_include_directories(vim-linux-pipelines PRIVATE common-interface ${CURSES_INCLUDE_DIRS} /usr/include/ncurses)
target_link_libraries(vim-linux-pipelines stdc++fs VimBackend ${CURSES_LIBRARIES} ${CURSES_PANEL_LIBRARY})

target_include_directories(example-linux-pipelines PRIVATE common-interface src ${CURSES_INCLUDE_DIRS} /usr/include/ncurses)
target_link_libraries(example-linux-pipelines stdc++fs ${CURSES_LIBRARIES} ${CURSES_PANEL_LIBRARY})

target_include_directories(nice-linux-pipelines PRIVATE common-interface ${CURSES_INCLUDE_DIRS} /usr/include/ncurses backends/backend-nice/src)
target_link_libraries(nice-linux-pipelines stdc++fs BackendNice ${CURSES_LIBRARIES} ${CURSES_PANEL_LIBRARY})