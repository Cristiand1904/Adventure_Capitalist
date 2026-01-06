cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(USE_EXTERNAL_GLFW OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)
# ---------------------------------------------------------------------

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        include/Business.h
        include/Player.h
        include/Game.h
        include/Upgrade.h
        include/Achievement.h
        include/Manager.h
        include/Wallet.h
        include/Exceptions.h
        include/Application.h
        src/Business.cpp
        src/Player.cpp
        src/Game.cpp
        src/Upgrade.cpp
        src/Achievement.cpp
        src/Manager.cpp
        src/Wallet.cpp
        src/Application.cpp
)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

if(WIN32)
    target_link_options(${MAIN_EXECUTABLE_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
endif()

target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE raylib)

if(WIN32)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE
        winmm
        gdi32
        user32
        shell32
        kernel32
        opengl32
    )
endif()

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
copy_files(FILES date_intrare.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})