#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <map>
#include <SDL2_image/SDL_image.h>

class Texture
{
	unsigned int m_texture;

public:

	Texture(std::string n_path);

	void Bind();
};

class TextureManager
{
	std::map<std::string, Texture*> m_map;
public:

	// also creates the texture
	void AddTexture(std::string n_name);

	// the name must be relative to the resource path
	void AddTexture(std::string n_name, Texture* n_texture);
	Texture* GetTexture(std::string n_name);
};

extern TextureManager g_textureManager;

#endif