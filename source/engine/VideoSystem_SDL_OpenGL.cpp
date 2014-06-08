#include "VideoSystem_SDL_OpenGL.h"
#include "Error.h"
#include "Event.h"
#include "Application.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/gl.h>

bool VideoSystem_SDL_OpenGL::VInit()
{
	PEDANTIC_DEBUG_MESSAGE("start SDL VInit()");

	const int TMP_SCREEN_WIDTH = 640;
	const int TMP_SCREEN_HEIGHT = 480;

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
										TMP_SCREEN_WIDTH,
										TMP_SCREEN_HEIGHT,
										SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN );
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

		SDL_GL_MakeCurrent(m_window,m_glContext);
	}

	glViewport( 0.f,0.f,TMP_SCREEN_WIDTH,TMP_SCREEN_HEIGHT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum(-5.0,5.0,-5.0,5.0,1.0,60.0);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClearColor(0.0f,0.0f,0.0f,1.0f);

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

void VideoSystem_SDL_OpenGL::VTranslateInput()
{
	SDL_Event tmp_event;

	while( SDL_PollEvent(&tmp_event) )
	{
		if( tmp_event.type == SDL_QUIT )
		{
			g_application->Kill();
		}
		else if( tmp_event.type == SDL_KEYDOWN )
		{
			if( tmp_event.key.keysym.sym >= SDLK_a && tmp_event.key.keysym.sym <= SDLK_z )
			{
				GetEventManager()->SendEvent(std::shared_ptr<Event_Input_Key_Down>(new Event_Input_Key_Down((Event_Input_Key)(A+tmp_event.key.keysym.sym - SDLK_a))));
			}

		}
	}
}