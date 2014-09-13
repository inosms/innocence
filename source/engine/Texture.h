#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <SDL2_image/SDL_image.h>

class Texture
{
	unsigned int m_texture;

public:

	Texture(std::string n_path);

	void Bind();

	void VRender();

};

#endif