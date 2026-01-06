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

if(DEFINED ENV{GITHUB_ACTIONS})
    message(STATUS "Building in CI/CD mode - headless only")
    set(HEADLESS_BUILD ON)
else()
    set(HEADLESS_BUILD OFF)
endif()

if(NOT HEADLESS_BUILD)
    include(FetchContent)
    FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.2
    )
    FetchContent_MakeAvailable(SFML)
endif()

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        include/Business.h
        include/StandardBusiness.h
        include/Player.h
        include/Game.h
        include/Upgrade.h
        include/Achievement.h
        include/Manager.h
        include/Wallet.h
        include/Exceptions.h
        include/Application.h
        src/Business.cpp
        src/StandardBusiness.cpp
        src/Player.cpp
        src/Game.cpp
        src/Upgrade.cpp
        src/Achievement.cpp
        src/Manager.cpp
        src/Wallet.cpp
        src/Application.cpp
)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

if(NOT HEADLESS_BUILD)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
endif()

if(WIN32 AND NOT HEADLESS_BUILD)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE sfml-main)
    target_link_options(${MAIN_EXECUTABLE_NAME} PRIVATE -static -static-libgcc -static-libstdc++ -mconsole)
endif()

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
copy_files(FILES date_intrare.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})