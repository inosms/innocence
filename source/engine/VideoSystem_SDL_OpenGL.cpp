#include "VideoSystem_SDL_OpenGL.h"
#include "Error.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/gl.h>

bool VideoSystem_SDL_OpenGL::VInit()
{
	PEDANTIC_DEBUG_MESSAGE("start SDL VInit()");

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		ERROR_MESSAGE("SDL could not be initialized: " << SDL_GetError() );
		return false;
	}
	else 
	{
		DEBUG_MESSAGE("SDL_init OK");
		m_window = SDL_CreateWindow( 	"window_test",
										SDL_WINDOWPOS_UNDEFINED,
										SDL_WINDOWPOS_UNDEFINED,
										640,
										480,
										SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if( !m_window ) 
		{
			ERROR_MESSAGE( "could not create SDL_Window ");
			return false;
		}

		m_glContext = SDL_GL_CreateContext(m_window);

		if( !m_glContext )
		{
			ERROR_MESSAGE( "could not create SDL_GL_Context" );
			return false;
		}
	}

	glClearColor(1,0,0,1);

	DEBUG_MESSAGE( "SDL Init successful" );


	return true;
}

bool VideoSystem_SDL_OpenGL::VExit()
{
	SDL_GL_DeleteContext( m_glContext );
	SDL_DestroyWindow( m_window );
	SDL_Quit();

	return true;
}

void VideoSystem_SDL_OpenGL::VClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void VideoSystem_SDL_OpenGL::VUpdateScreen()
{
	SDL_GL_SwapWindow(m_window);
}