cmake_minimum_required(VERSION 3.16)
project(Project_3__Minesweeper LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(Project_3__Minesweeper main.cpp
        board.h)
target_link_libraries(Project_3__Minesweeper PRIVATE sfml-graphics)
target_compile_features(Project_3__Minesweeper PRIVATE cxx_std_11)

if(WIN32)
    add_custom_command(
            TARGET Project_3__Minesweeper
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Project_3__Minesweeper>
            VERBATIM)
endif()

install(TARGETS Project_3__Minesweeper)