#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <map>
#include <SDL2_image/SDL_image.h>

class Texture
{
	unsigned int m_texture;
	unsigned int m_height, m_width;

	void InitFromSurface(SDL_Surface* n_surface);

public:

	Texture(std::string n_path);
	Texture(SDL_Surface* n_surface);

	void Bind();
	
	// binds the texture to GL_TEXTURE0+n_toWhichActive
	void Bind(int n_toWhichActive);

	unsigned int GetHeight();
	unsigned int GetWidth();
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