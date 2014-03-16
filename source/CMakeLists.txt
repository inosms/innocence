set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build/ )
cmake_minimum_required (VERSION 2.6) 
project(innocence)

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
	FIND_LIBRARY( OPENGL_LIBRARY OpenGL )
ENDIF()

add_subdirectory(engine)

add_executable(innocence main.cpp)
target_link_libraries( innocence engine ${SDL2_LIBRARY} ${OPENGL_LIBRARY} )