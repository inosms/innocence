#ifndef _VIDEOSYSTEM_SDL_OPENGL_H_
#define _VIDEOSYSTEM_SDL_OPENGL_H_

#include "VideoSystem.h"
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

class VideoSystem_SDL_OpenGL : public VideoSystem
{
	SDL_Window* m_window;
	SDL_GLContext m_glContext;

public:
	virtual bool VInit();
	virtual bool VExit();

	virtual void VClearScreen();
	virtual void VUpdateScreen();

	virtual void VTranslateInput();
};

#endif