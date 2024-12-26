cmake_minimum_required(VERSION 3.28)
project(learning_more_cpp_idioms)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE Debug)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/opt/llvm/lib/c++")

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
message("thirdparty dir: " ${THIRDPARTY_DIR})

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# import cpp_utils
FetchContent_Declare(
    cpp_utils
    GIT_REPOSITORY https://github.com/xuexcy/cpp_utils.git
    GIT_TAG main
    SOURCE_DIR ${THIRDPARTY_DIR}/cpp_utils
)
FetchContent_MakeAvailable(cpp_utils)
include_directories(${THIRDPARTY_DIR}/cpp_utils)
set(EXTRA_LIBS ${EXTRA_LIBS} cpp_utils)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin)

add_executable(02 src/02/main.cc src/02/game_player.cpp)

add_executable(03 src/03/main.cc)

add_executable(04 src/04/main.cc)
add_library(directory src/04/directory.cpp)
add_library(file_system src/04/file_system.cpp)
target_link_libraries(directory file_system)
target_link_libraries(04 directory cpp_utils)

add_executable(05 src/05/main.cc)
add_executable(05_2 src/05/main2.cc)

add_executable(06 src/06/main.cc)

add_executable(07 src/07/main.cc)

add_executable(08 src/08/main.cc)

add_executable(09 src/09/main.cc)

add_executable(10 src/10/main.cc)

add_executable(11 src/11/main.cc)

add_executable(12 src/12/main.cc)

add_executable(13 src/13/main.cc)

add_executable(14 src/14/main.cc)

add_executable(15 src/15/main.cc)

add_executable(17 src/17/main.cc)

add_executable(18 src/18/main.cc)

add_executable(20 src/20/main.cc)

add_executable(21 src/21/main.cc)

add_executable(22 src/22/main.cc)

add_executable(23 src/23/main.cc)

add_executable(24 src/24/main.cc)

add_executable(25 src/25/main.cc)
