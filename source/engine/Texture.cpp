#include "Texture.h"
#include "SDL2/SDL.h"
#include "engine.h"

TextureManager g_textureManager;

Texture::Texture(std::string n_path) : Texture(IMG_Load(n_path.c_str()))
{
	SDL_Surface* tmp_surface = IMG_Load(n_path.c_str());
	if( !tmp_surface ) throw Exception("could not load image" + n_path);
	InitFromSurface(tmp_surface);
}

// https://stackoverflow.com/questions/18667178/opengl-texture-from-sdl-surface
void Texture::InitFromSurface(SDL_Surface* n_surface)
{
	if( !n_surface ) throw Exception("surface must not be null" );

    glEnable(GL_TEXTURE_2D);

    GLenum Mode = 0;
    switch (n_surface->format->BytesPerPixel) 
    {
	    case 1: { Mode = GL_ALPHA; break; }
	    case 3: { Mode = GL_RGB; break; }
	    case 4: { Mode = GL_RGBA; break; }
	    default: { break; }
	}
	
    glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, n_surface->w, n_surface->h, 0, Mode, GL_UNSIGNED_BYTE, n_surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_height = n_surface->h;
    m_width = n_surface->w;

    glDisable(GL_TEXTURE_2D);
}


Texture::Texture(SDL_Surface* n_surface)
{
	InitFromSurface(n_surface);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::Bind(int n_toWhichActive)
{
	glActiveTexture(GL_TEXTURE0+n_toWhichActive);
	Bind();
}


unsigned int Texture::GetHeight()
{
	return m_height;
}
unsigned int Texture::GetWidth()
{
	return m_width;
}

Texture::~Texture()
{
	glDeleteTextures(1,&m_texture);
}

void TextureManager::AddTexture(std::string n_name)
{
	AddTexture(n_name,new Texture(g_application->m_resourcePath+n_name));
}

void TextureManager::AddTexture(std::string n_name, Texture* n_texture)
{
	m_map[n_name] = n_texture;
}


Texture* TextureManager::GetTexture(std::string n_name)
{
	if( m_map.find(n_name) == m_map.end() )
		return nullptr;
	else
		return m_map[n_name];
}

bool TextureManager::RemoveTexture(std::string n_name)
{
	auto tmp_find = m_map.find(n_name);
	if( tmp_find == m_map.end() ) return false;

	Texture* tmp_texture = tmp_find->second;
	m_map.erase(tmp_find);
	delete tmp_texture;
	return true;
}
