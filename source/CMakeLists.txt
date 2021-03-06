set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build/ )
cmake_minimum_required (VERSION 2.6)
project(innocence)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

# http://stackoverflow.com/questions/9531637/cmake-command-line-for-c-define
# if per command line -DPEDANTIC-DEBUG=true is typed; this code is executed and
# pedantic debug is activated. YEAH!
IF( DEFINED PEDANTIC_DEBUG )
 add_definitions( -DPEDANTIC_DEBUG )
ENDIF()

IF( DEFINED DEBUG )
 add_definitions( -DDEBUG )
ENDIF()

# http://www.cmake.org/Wiki/CMake:HowToUseExistingOSXFrameworks
IF( APPLE )
	FIND_LIBRARY( SDL2_LIBRARY SDL2 )
	FIND_LIBRARY( SDL2IMAGE_LIBRARY SDL2_image )
	FIND_LIBRARY( SDL2TTF_LIBRARY SDL2_ttf )
	FIND_LIBRARY( OPENGL_LIBRARY OpenGL )
ENDIF()
INCLUDE(FindPkgConfig)
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

include_directories("engine/Box2D/")
add_subdirectory(engine)
add_subdirectory(engine/Box2D/Box2D)
add_subdirectory(game)

add_executable(innocence main.cpp)
IF(APPLE)
    target_link_libraries( innocence engine game Box2D pugixml assimp ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2TTF_LIBRARY} ${OPENGL_LIBRARY} )
ELSE()
    target_link_libraries( innocence game engine Box2D pugixml assimp SDL2 SDL2_image SDL2_ttf ${OPENGL_LIBRARIES} )
ENDIF()
