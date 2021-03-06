#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <map>
#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#elif __linux__
#include <SDL2/SDL_image.h>
#endif
class Texture
{
	unsigned int m_texture;
	unsigned int m_height, m_width;

	void InitFromSurface(SDL_Surface* n_surface);

public:

	Texture(std::string n_path);
	Texture(SDL_Surface* n_surface);
	// create this texture object from an already existing OpenGL texture
	Texture(unsigned int n_id,unsigned int n_height, unsigned int n_width);
	~Texture();

	void Bind();

	// binds the texture to GL_TEXTURE0+n_toWhichActive
	void Bind(int n_toWhichActive);

	unsigned int GetHeight();
	unsigned int GetWidth();

	unsigned int GetId();

	// for FBOs
	// n_scale scales the size of the texture
	// 1 = the current window size
	static Texture* GetColorAttachmentTexture(float n_scale = 1.0f);
	static Texture* GetDepthAttachmentTexture(float n_scale = 1.0f);
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

	// returns true if deleted
	// returns false if this texture was not found
	bool RemoveTexture(std::string n_name);
};

extern TextureManager g_textureManager;

#endif
