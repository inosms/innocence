#include "Error.h"
#include "Font.h"
#include "Application.h"

FontManager g_fontManager;

Font::Font(std::string n_path, int n_size)
{
	m_font = TTF_OpenFont(n_path.c_str(),n_size);
	if( !m_font ) throw Exception(std::string("Failed to load font! SDL_ttf Error: ") + TTF_GetError() );
}

Texture* Font::GetTextureForText(std::string n_text, SDL_Color n_color)
{
	SDL_Surface* tmp_textSurface = TTF_RenderText_Blended(m_font,n_text.c_str(),n_color);
	if( !tmp_textSurface ) throw Exception(std::string("Unable to render text surface! SDL_ttf Error: ") + TTF_GetError() );
	Texture* tmp_texture = new Texture(tmp_textSurface);
	SDL_FreeSurface(tmp_textSurface);
	return tmp_texture;
}

Font::~Font()
{
	TTF_CloseFont(m_font); 
	m_font = nullptr;
}

void FontManager::AddFont(std::string n_path, int n_size)
{
	Font* tmp_newFont = new Font(g_application->m_resourcePath+n_path,n_size);
	AddFont(n_path,tmp_newFont);
}

void FontManager::AddFont(std::string n_name, Font* n_font)
{
	m_map[n_name] = n_font;
}

Font* FontManager::GetFont(std::string n_name)
{
	if( m_map.find(n_name) == m_map.end() )
		return nullptr;
	else
		return m_map[n_name];
}