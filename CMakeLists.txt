cmake_minimum_required(VERSION 3.20)
project(oop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
endif()

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

file(GLOB SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

set(MAIN_SOURCE "${CMAKE_SOURCE_DIR}/main.cpp")

file(GLOB_RECURSE HEADERS
        "${CMAKE_SOURCE_DIR}/include/*.h"
)

add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${SOURCES} ${HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/artifacts/bin"
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} raylib)

if(APPLE)
    target_link_libraries(${PROJECT_NAME}
            "-framework IOKit"
            "-framework Cocoa"
            "-framework OpenGL"
    )
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm gdi32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE FALSE
    )
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl rt)
endif()