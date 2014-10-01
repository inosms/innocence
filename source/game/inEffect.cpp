#include "inEffect.h"

// before applying the blur effect resize the texture
// to render faster (the details won't matter anyway)
const float EFFECT_BLUR_RESIZE_FACTOR = 0.3f;

Effect_BlurHorizontal::Effect_BlurHorizontal(Texture* n_inputTexture) : Effect(nullptr,EFFECT_BLUR_RESIZE_FACTOR)
{
    // set shader here and not in constructor as Init must be called
    m_shader = new Shader(GetResourcePath("effect_default_vertex.shader"),GetResourcePath(""),GetResourcePath("effect_blurHorizontal_fragment.shader"));
    m_shader->Init();
    AddInputTexture("texture_screen",n_inputTexture);
}

void Effect_BlurHorizontal::VSetShader()
{
    m_shader->SetFloat("screen_width",GetVideoSystem()->VGetWidth()*EFFECT_BLUR_RESIZE_FACTOR);
}

Effect_BlurVertical::Effect_BlurVertical(Texture* n_inputTexture) : Effect(nullptr,EFFECT_BLUR_RESIZE_FACTOR)
{
    // set shader here and not in constructor as Init must be called
    m_shader = new Shader(GetResourcePath("effect_default_vertex.shader"),GetResourcePath(""),GetResourcePath("effect_blurVertical_fragment.shader"));
    m_shader->Init();
    AddInputTexture("texture_screen",n_inputTexture);
}

void Effect_BlurVertical::VSetShader()
{
    m_shader->SetFloat("screen_height",GetVideoSystem()->VGetHeight()*EFFECT_BLUR_RESIZE_FACTOR);
}

Effect_Bloom::Effect_Bloom(Texture* n_inputTextureBlurred, Texture* n_inputTexture) : Effect(nullptr,1.f)
{
    // set shader here and not in constructor as Init must be called
    m_shader = new Shader(GetResourcePath("effect_default_vertex.shader"),GetResourcePath(""),GetResourcePath("effect_bloom_fragment.shader"));
    m_shader->Init();
    AddInputTexture("texture_screen",n_inputTexture);
    AddInputTexture("texture_blurred",n_inputTextureBlurred);
}

Effect_DOF::Effect_DOF(Texture* n_inputTexture, Texture* n_inputDepthTexture) : Effect(nullptr,0.5f)
{
    m_shader = new Shader(GetResourcePath("effect_default_vertex.shader"),GetResourcePath(""),GetResourcePath("effect_dof_fragment.shader"));
    m_shader->Init();
    AddInputTexture("bgl_RenderedTexture",n_inputTexture);
    AddInputTexture("bgl_DepthTexture",n_inputDepthTexture);
}

void Effect_DOF::VSetShader()
{
    m_shader->SetFloat("bgl_RenderedTextureWidth",GetVideoSystem()->VGetWidth()*0.5f);
    m_shader->SetFloat("bgl_RenderedTextureHeight",GetVideoSystem()->VGetHeight()*0.5f);
}
