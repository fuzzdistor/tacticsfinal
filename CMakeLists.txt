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

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}/extlibs/sfml/include"
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

add_executable("${CMAKE_PROJECT_NAME}"
    src/application.cpp
    src/board.cpp
    src/map.cpp
    include/board.hpp
    include/map.hpp
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

if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE ${SFML_LIBRARIES})
endif()
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE ${SFML_DEBUG_LIBRARIES})
endif()

if(MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-main)
endif()

