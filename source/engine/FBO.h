#ifndef _FBO_H_
#define _FBO_H_

#include <OpenGL/gl3.h>

// http://www.lighthouse3d.com/tutorials/opengl-short-tutorials/opengl_framebuffer_objects/

enum class FBO_Texture_Attachment
{
    COLOR,
    DEPTH,
    STENCIL,
    DEPTH_STENCIL
};
class Texture;
class FBO
{
    unsigned int m_id, m_width, m_height;
    unsigned int m_colorAttachmentCount = 0;
public:
    FBO(unsigned int n_width, unsigned int n_height);
    void Bind();
    void UnBind();

    void AddTexture(Texture* n_texture,FBO_Texture_Attachment n_attachment);
};

#endif
