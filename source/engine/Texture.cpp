#include "Texture.h"
#include "SDL2/SDL.h"
#include "engine.h"

TextureManager g_textureManager;

// https://stackoverflow.com/questions/18667178/opengl-texture-from-sdl-surface
Texture::Texture(std::string n_path)
{
	SDL_Surface* Surface = IMG_Load(n_path.c_str());
	if( !Surface ) throw Exception("could not load image " + n_path );

    glEnable(GL_TEXTURE_2D);

    GLenum Mode = 0;
    switch (Surface->format->BytesPerPixel) 
    {
	    case 1: { Mode = GL_ALPHA; break; }
	    case 3: { Mode = GL_RGB; break; }
	    case 4: { Mode = GL_RGBA; break; }
	    default: { break; }
	}

    glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDisable(GL_TEXTURE_2D);
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
