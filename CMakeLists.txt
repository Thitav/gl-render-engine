cmake_minimum_required(VERSION 3.0.0)
project(gl-game-engine VERSION 0.1.0 LANGUAGES C)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
file (GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")

set (INCLUDE_DIRS "")
foreach (_headerFile ${HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES INCLUDE_DIRS)

add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/cglm EXCLUDE_FROM_ALL)
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)
include_directories(lib/glad/include lib/stb lib/assimp/include)

add_executable(gl-game-engine lib/glad/src/glad.c ${SOURCES})
target_include_directories(gl-game-engine PRIVATE ${INCLUDE_DIRS})
target_link_libraries(gl-game-engine PRIVATE glfw cglm assimp)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
