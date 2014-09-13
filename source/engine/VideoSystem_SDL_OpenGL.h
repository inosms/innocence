#ifndef _VIDEOSYSTEM_SDL_OPENGL_H_
#define _VIDEOSYSTEM_SDL_OPENGL_H_

#include "VideoSystem.h"
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

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

	virtual void VTranslateInput();
	SDL_Renderer* GetRenderer();
};

#endif