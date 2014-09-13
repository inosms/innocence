#include "VideoSystem_SDL_OpenGL.h"
#include "Error.h"
#include "Event.h"
#include "Application.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <OpenGL/gl3.h>
#include "Shader.h"
#include "Math.h"

void VideoSystem_SDL_OpenGL::VInit()
{
	PEDANTIC_DEBUG_MESSAGE("start SDL VInit()");

	const int TMP_SCREEN_WIDTH = 640;
	const int TMP_SCREEN_HEIGHT = 480;

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) throw Exception( std::string("SDL could not be initialized: ") + SDL_GetError() );

	DEBUG_MESSAGE("SDL_init OK");

	// Load SDL_image
	int flags=IMG_INIT_JPG|IMG_INIT_PNG;
	int initted=IMG_Init(flags);
	if((initted&flags) != flags) throw Exception(std::string("Failed to load SDL_Image") + std::string(IMG_GetError()));

	SDL_CreateWindowAndRenderer(	TMP_SCREEN_WIDTH,
									TMP_SCREEN_HEIGHT,
									SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI, &m_window,&m_renderer);
	// https://bugzilla.libsdl.org/show_bug.cgi?id=1934

	// https://stackoverflow.com/questions/23630096/only-glsl-shader-version-120-works-on-mac-os-x
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// TODO: glew?

	if( !m_window ) throw Exception("could not create SDL_Window");
	if( !m_renderer ) throw Exception("could not create SDL_Renderer");
	
	m_glContext = SDL_GL_CreateContext(m_window);

	if( !m_glContext ) throw Exception( "could not create SDL_GL_Context" );

	SDL_GL_MakeCurrent(m_window,m_glContext);
	SDL_GL_SetSwapInterval(1);


	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glViewport( 0.f,0.f,TMP_SCREEN_WIDTH,TMP_SCREEN_HEIGHT );

	// https://stackoverflow.com/questions/22561832/gl-depth-test-does-not-work-in-opengl-3-3
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	DEBUG_MESSAGE( "SDL Init successful" );
}

void VideoSystem_SDL_OpenGL::VExit()
{
	SDL_GL_DeleteContext( m_glContext );
	SDL_DestroyWindow( m_window );
	SDL_Quit();
}

void VideoSystem_SDL_OpenGL::VClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		else if( tmp_event.type == SDL_MOUSEBUTTONDOWN )
		{
			if( tmp_event.button.button == SDL_BUTTON_LEFT)
			{
				int tmp_height = 1; int tmp_width = 1;
				SDL_GetWindowSize(m_window,&tmp_width,&tmp_height);

				GetEventManager()->SendEvent( std::shared_ptr<Event_Input_Mousebutton_Down>(new Event_Input_Mousebutton_Down(LEFT,tmp_event.button.x,tmp_event.button.y,tmp_event.button.x/float(tmp_width),tmp_event.button.y/float(tmp_height))));
			}
		}
		else if( tmp_event.type == SDL_MOUSEMOTION )
		{
			int tmp_height = 1; int tmp_width = 1;
			SDL_GetWindowSize(m_window,&tmp_width,&tmp_height);

			GetEventManager()->SendEvent( std::shared_ptr<Event_Input_Mousemotion> (new Event_Input_Mousemotion(tmp_event.motion.x, tmp_event.motion.y, tmp_event.motion.x/float(tmp_width), tmp_event.motion.y/float(tmp_height))));
		}
	}
}

SDL_Renderer* VideoSystem_SDL_OpenGL::GetRenderer()
{
	return m_renderer;
}
