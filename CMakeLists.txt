cmake_minimum_required(VERSION 3.16)
project(TACTICSFINAL LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# if no -DCMAKE_BUILD_TYPE flag was passed default to debug
if(NOT CMAKE_BUILD_TYPE)
    MESSAGE("Build type not set. Defaulting to DEBUG")
    set(CMAKE_BUILD_TYPE DEBUG)
else()
    MESSAGE("Build type set to ${CMAKE_BUILD_TYPE}")
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILDTYPE)

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}/extlibs/sfml/include"
  SYSTEM "${CMAKE_SOURCE_DIR}/extlibs/ImGUI"
)

set(SFML_BUILD_GRAPHICS TRUE)
set(SFML_BUILD_WINDOW TRUE)
set(SFML_BUILD_SYSTEM TRUE)
set(SFML_BUILD_AUDIO TRUE)
set(SFML_BUILD_NETWORK FALSE)
set(SFML_BUILD_EXAMPLES FALSE)

if(MSVC)
    set(BUILD_SHARED_LIBS FALSE)
    add_definitions(-DSFML_STATIC)
else()
    set(BUILD_SHARED_LIBS TRUE)
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/extlibs/sfml")
add_subdirectory("${CMAKE_SOURCE_DIR}/extlibs/ImGUI")

add_executable("${CMAKE_PROJECT_NAME}"
    src/application.cpp
    src/ai.cpp
    src/astar.cpp
    src/astarnode.cpp
    src/battlescene.cpp
    src/board.cpp
    src/combatmanager.cpp
    src/cursor.cpp
    src/game.cpp
    src/map.cpp
    src/scene.cpp
    src/team.cpp
    src/tilemarkers.cpp
    src/turnmanager.cpp
    src/tweener.cpp
    src/unit.cpp
    src/xorshift.cpp
    include/ai.hpp
    include/astar.hpp
    include/astarnode.hpp
    include/battlescene.hpp
    include/board.hpp
    include/combatmanager.hpp
    include/cursor.hpp
    include/easingfunctions.hpp
    include/game.hpp
    include/map.hpp
    include/unit.hpp
    include/scene.hpp
    include/stats.hpp
    include/team.hpp
    include/tilemarkers.hpp
    include/turnmanager.hpp
    include/tween.hpp
    include/tweener.hpp
    include/resourcemanager.hpp
    include/xorshift.hpp
    )

if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -pedantic
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wmisleading-indentation
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wuseless-cast
    -Wdouble-promotion
    -Wformat=2
  )
endif(CMAKE_COMPILER_IS_GNUCXX)

set(COMMON_LIBARARIES_STATIC
    opengl32
    freetype
    winmm
    gdi32
    flac
    vorbisenc
    vorbisfile
    vorbis
    ogg
)

if (NOT SFML_BUILD_STATIC)
  set(SFML_DEBUG_LIBRARIES
    sfml-graphics-d
    sfml-window-d
    sfml-system-d
    sfml-audio-d
  )

  set(SFML_LIBRARIES
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
  )
else ()

  set(SFML_DEBUG_LIBRARIES
    sfml-graphics-s-d
    sfml-window-s-d
    sfml-system-s-d
    sfml-audio-s-d
    ${COMMON_LIBRARIES_STATIC}
  )

  set(SFML_LIBRARIES
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    sfml-audio-s
    ${COMMON_LIBRARIES_STATIC}
  )
endif()

if(BUILDTYPE MATCHES RELEASE)
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE ${SFML_LIBRARIES} imgui)
endif()
if(BUILDTYPE MATCHES DEBUG)
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE ${SFML_DEBUG_LIBRARIES} imgui)
endif()

if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS" "/MP")
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-main imgui)
endif()

