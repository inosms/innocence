#include "FBO.h"
#include "Texture.h"

FBO::FBO(unsigned int n_width, unsigned int n_height) :
    m_width(n_width),
    m_height(n_height)
{
    glGenFramebuffers(1, &m_id);
}

void FBO::Bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);

}

void FBO::UnBind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FBO::AddTexture(Texture* n_texture,FBO_Texture_Attachment n_attachment)
{
    Bind();
    if( n_attachment == FBO_Texture_Attachment::COLOR )
    {
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+m_colorAttachmentCount++, n_texture->GetId(), 0);
        GLuint* attachments = new GLuint(m_colorAttachmentCount);
        for(unsigned int i = 0; i < m_colorAttachmentCount; i++ )
            attachments[i] = GL_COLOR_ATTACHMENT0+i;
        glDrawBuffers(m_colorAttachmentCount,  attachments);
        delete[] attachments;
    }
    else if( n_attachment == FBO_Texture_Attachment::DEPTH )
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, n_texture->GetId(), 0);
    else if( n_attachment == FBO_Texture_Attachment::STENCIL )
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, n_texture->GetId(), 0);
    else if( n_attachment == FBO_Texture_Attachment::DEPTH_STENCIL )
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, n_texture->GetId(), 0);
    UnBind();
}
