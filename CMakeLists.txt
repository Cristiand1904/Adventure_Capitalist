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

if(DEFINED ENV{GITHUB_ACTIONS} AND NOT WIN32)
    message(STATUS "Building in CI/CD mode - headless only")
    set(HEADLESS_BUILD ON)
else()
    set(HEADLESS_BUILD OFF)
endif()

if(HEADLESS_BUILD)
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
            include/ApplicationHeadless.h
            src/Business.cpp
            src/StandardBusiness.cpp
            src/Player.cpp
            src/Game.cpp
            src/Upgrade.cpp
            src/Achievement.cpp
            src/Manager.cpp
            src/Wallet.cpp
            src/ApplicationHeadless.cpp
    )

    target_compile_definitions(${MAIN_EXECUTABLE_NAME} PRIVATE USE_HEADLESS_APP)
else()
    include(FetchContent)

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.2
    )
    FetchContent_MakeAvailable(SFML)

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

    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
endif()

if(WIN32 AND MSVC)
    set_compiler_flags(RUN_SANITIZERS FALSE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
    target_link_options(${MAIN_EXECUTABLE_NAME} PRIVATE /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:MSVCRTD)
else()
    set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
endif()

if(WIN32 AND NOT HEADLESS_BUILD)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE sfml-main)
    if(NOT MSVC)
        target_link_options(${MAIN_EXECUTABLE_NAME} PRIVATE -static -static-libgcc -static-libstdc++ -mconsole)
    endif()
endif()

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
copy_files(FILES date_intrare.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})