#include "Texture.h"
#include "SDL2/SDL.h"
#include "engine.h"

// https://stackoverflow.com/questions/18667178/opengl-texture-from-sdl-surface
unsigned int LoadImage(std::string n_path) 
{
	SDL_Surface* Surface = IMG_Load(n_path.c_str());
	if( !Surface ) throw Exception("could not load image " + n_path );

    glEnable(GL_TEXTURE_2D);
    unsigned int TextureID;

    GLenum Mode = 0;
    switch (Surface->format->BytesPerPixel) {
    case 1: {
        Mode = GL_ALPHA;
            break;
            }
    case 3: {
        Mode = GL_RGB;
        break;
            }
    case 4: {
        Mode = GL_RGBA;
        break;
            }
    default: { break;
             }
    }

    glGenTextures(1, &TextureID);

    glBindTexture(GL_TEXTURE_2D, TextureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDisable(GL_TEXTURE_2D);
    return TextureID;

}

Texture::Texture(std::string n_path)
{
	m_texture = LoadImage(n_path);
	std::cout << m_texture << " is new texture " << std::endl;
	/*
	SDL_Surface* tmp_loadedSurface = IMG_Load(n_path.c_str());
	if( !tmp_loadedSurface ) throw Exception("could not load image " + n_path );

	m_texture = SDL_CreateTextureFromSurface(dynamic_cast<VideoSystem_SDL_OpenGL*>(GetVideoSystem())->GetRenderer(),tmp_loadedSurface);
	if( !m_texture ) throw Exception("could not generate texture");

	SDL_FreeSurface(tmp_loadedSurface);*/
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
