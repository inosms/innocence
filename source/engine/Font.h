#ifndef _FONT_H_
#define _FONT_H_

#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_ttf/SDL_ttf.h>
#elif __linux__
#include <SDL2/SDL_ttf.h>
#endif
#include <string>
#include <map>
#include "Texture.h"

class Font
{
	TTF_Font* m_font;
public:
	Font(std::string n_path,int n_size);

	Texture* GetTextureForText(std::string n_text, SDL_Color n_color);

	~Font();
};

class FontManager
{
	std::map<std::string, Font*> m_map;
public:
	// add font path relative to resourcepath
	void AddFont(std::string n_path, int n_size);
	// add font with name
	void AddFont(std::string n_name, Font* n_font);
	// get font by name
	Font* GetFont(std::string n_name);
};

extern FontManager g_fontManager;
#endif
