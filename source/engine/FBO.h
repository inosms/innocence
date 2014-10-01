#ifndef _FBO_H_
#define _FBO_H_

#include "VideoSystem_SDL_OpenGL.h"
#include <vector>

// http://www.lighthouse3d.com/tutorials/opengl-short-tutorials/opengl_framebuffer_objects/

enum class FBO_Texture_Attachment
{
    COLOR,
    DEPTH,
    STENCIL,
    DEPTH_STENCIL
};
class Texture;
class MeshTexture;
class FBO
{
    unsigned int m_id;
    unsigned int m_colorAttachmentCount = 0;

    // this is used for rendering
    MeshTexture* m_rect = nullptr;
    std::vector<Texture*> m_colorAttachments;
    Texture* m_depthAttachment;
    float m_scale;
public:
    // creates a FBO
    // n_scale scales the FBO to n_scales times the current window size
    // this requires all the textures to also have this scale!
    // (otherwise it either cuts off the image or adds black stripes)
    FBO(float n_scale = 1.0f);
    ~FBO();
    void Bind();
    void UnBind();

    // sets the default values; call this every time the screen resolution changes
    void Init();

    void CheckState();

    // add a texture; many color textures can be added
    // but only one depth texture!
    // all textures are deleted by this FBO!
    void AddTexture(Texture* n_texture,FBO_Texture_Attachment n_attachment);

    // just a shortcut for adding texture; this creates a texture and stores it here
    // adds a new color texture and returns the number of texture
    // this number can be used with Render(uint) to render the color texture
    // and can also be used with GLSL to output to this texture
    // the added textue is also scaled with m_scale
    unsigned int AddColorTexture();
    // adds a depth texture, as only one depth texture is allowed this returns nothing
    // is also scaled with m_scale
    void AddDepthTexture();

    // render color attachment to screen; specify number here
    // 0 is the first added
    // 1 is the second added,
    // etc
    void Render(unsigned int n_colorAttachmentNumber);

    // returns the texture with the given attachmentNumber
    // or nullptr if not existing
    Texture* GetTexture(unsigned int n_colorAttachmentNumber);
    Texture* GetDepthTexture();
};

#endif
