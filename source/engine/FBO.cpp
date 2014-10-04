#include "FBO.h"
#include "Texture.h"
#include "Error.h"
#include "Application.h"
#include "Shader.h"

FBO::FBO(float n_scale) :
m_scale(n_scale)
{
    if( g_shaderManager.GetShader("fbo") == nullptr )
        g_shaderManager.AddShader("fbo");

    // create a 1x1 rect with no texture yet
    m_rect = Mesh::GetTexturedRect(1,1,0.5,0.5,nullptr,1,-1);

    glGenFramebuffers(1, &m_id);
}

FBO::~FBO()
{
    glDeleteFramebuffers(1,&m_id);
    for( auto i : m_colorAttachments )
        delete i;
    delete m_depthAttachment;
}
void FBO::Bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);

    // applying the scale
    glViewport( 0.f,0.f,GetVideoSystem()->VGetWidth()*m_scale,GetVideoSystem()->VGetHeight()*m_scale );

}

void FBO::UnBind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // for resetting the scale
    glViewport( 0.f,0.f,GetVideoSystem()->VGetWidth(),GetVideoSystem()->VGetHeight() );
}

void FBO::CheckState()
{
    Bind();
    GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	switch (e) {

		case GL_FRAMEBUFFER_UNDEFINED:
			printf("FBO Undefined\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
			printf("FBO Incomplete Attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT :
			printf("FBO Missing Attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
			printf("FBO Incomplete Draw Buffer\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED :
			printf("FBO Unsupported\n");
			break;
		case GL_FRAMEBUFFER_COMPLETE:
			printf("FBO OK\n");
			break;
		default:
			printf("FBO Problem?\n");
	}
    UnBind();
}

void FBO::AddTexture(Texture* n_texture,FBO_Texture_Attachment n_attachment)
{
    Bind();
    if( n_attachment == FBO_Texture_Attachment::COLOR )
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+m_colorAttachmentCount++, n_texture->GetId(), 0);
        GLuint* attachments = new GLuint(m_colorAttachmentCount);
        for(unsigned int i = 0; i < m_colorAttachmentCount; i++ )
            attachments[i] = GL_COLOR_ATTACHMENT0+i;
        glDrawBuffers(m_colorAttachmentCount,  attachments);
        delete[] attachments;
        m_colorAttachments.push_back(n_texture);
    }
    else if( n_attachment == FBO_Texture_Attachment::DEPTH )
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, n_texture->GetId(), 0);
        m_depthAttachment = n_texture;
    }
    else if( n_attachment == FBO_Texture_Attachment::STENCIL )
        glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, n_texture->GetId(), 0);
    else if( n_attachment == FBO_Texture_Attachment::DEPTH_STENCIL )
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, n_texture->GetId(), 0);
    UnBind();
}

unsigned int FBO::AddColorTexture()
{
    AddTexture(Texture::GetColorAttachmentTexture(m_scale),FBO_Texture_Attachment::COLOR);
    return m_colorAttachmentCount-1;
}

void FBO::AddDepthTexture()
{
    AddTexture(Texture::GetDepthAttachmentTexture(m_scale),FBO_Texture_Attachment::DEPTH);
}

Texture* FBO::GetDepthTexture()
{
    return m_depthAttachment;
}


void FBO::Render(unsigned int n_colorAttachmentNumber)
{
    static Shader* tmp_shader = g_shaderManager.GetShader("fbo");
    tmp_shader->Begin();
    tmp_shader->SetTexture("tex",0);
    tmp_shader->SetMat("modelview",glm::mat4x4());
    tmp_shader->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
    m_rect->SetTexture(m_colorAttachments[n_colorAttachmentNumber]);
    m_rect->Render();
    // reset, as the FBO handles the deletion of the textures and NOT the Mesh!!
    m_rect->SetTexture(nullptr);
    tmp_shader->End();
}

Texture* FBO::GetTexture(unsigned int n_colorAttachmentNumber)
{
    if( n_colorAttachmentNumber >= m_colorAttachments.size() ) return nullptr;
    else return m_colorAttachments[n_colorAttachmentNumber];
}
