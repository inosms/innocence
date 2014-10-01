#ifndef _VIDEOSYSTEM_SDL_OPENGL_H_
#define _VIDEOSYSTEM_SDL_OPENGL_H_

#include "VideoSystem.h"
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class VideoSystem_SDL_OpenGL : public VideoSystem
{
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_GLContext m_glContext;

public:
	virtual void VInit();
	virtual void VExit();

	virtual void VClearScreen();
	virtual void VUpdateScreen();

	virtual unsigned int VGetWidth();
	virtual unsigned int VGetHeight();

	virtual void VTranslateInput();
	SDL_Renderer* GetRenderer();
};

#endif
