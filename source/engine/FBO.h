#ifndef _FBO_H_
#define _FBO_H_

#include <OpenGL/gl3.h>
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
public:
    FBO();
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
    unsigned int AddColorTexture();
    // adds a depth texture, as only one depth texture is allowed this returns nothing
    void AddDepthTexture();

    // render color attachment to screen; specify number here
    // 0 is the first added
    // 1 is the second added,
    // etc
    void Render(unsigned int n_colorAttachmentNumber);

    // returns the texture with the given attachmentNumber
    // or nullptr if not existing
    Texture* GetTexture(unsigned int n_colorAttachmentNumber);
};

#endif
