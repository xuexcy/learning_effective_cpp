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
