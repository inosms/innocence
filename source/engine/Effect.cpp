#include "Effect.h"
#include "engine.h"

Effect::Effect(Shader* n_shader, float n_scale)
{
    m_shader = std::unique_ptr<Shader>(n_shader);
    m_fbo = std::unique_ptr<FBO>(new FBO(n_scale));
    m_fbo->AddColorTexture();
    m_fbo->AddDepthTexture();
    m_fbo->CheckState();

    m_rect = std::unique_ptr<Mesh>(Mesh::GetTexturedRect(1,1,0.5,0.5,nullptr,1,-1));
}

Effect::~Effect(){}

// adds a texture to the input;
// the texture is NOT deleted with the effect,
// its just a pointer to a texture somewhere (most likely a FBO)
void Effect::AddInputTexture(std::string n_glslName, Texture* n_texture)
{
    m_inputTextures[n_glslName] = n_texture;
}

Texture* Effect::GetResultTexture()
{
    return m_fbo->GetTexture(0);
}

void Effect::Apply()
{
    m_fbo->Bind();
        GetVideoSystem().VClearScreen();
        m_shader->Begin();
        VSetShader();

        // bind all the textures
        int tmp_counter = 0;
        for( auto i : m_inputTextures )
        {
            m_shader->SetTexture(i.first,tmp_counter);
            i.second->Bind(tmp_counter);
            tmp_counter++;
        }

        m_shader->SetMat("modelview",glm::mat4x4());
        m_shader->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
        m_rect->Render();
        m_rect->SetTexture(nullptr);
    m_fbo->UnBind();
}

void Effect::RenderResult()
{
    m_fbo->Render(0);
}

void Effect::SetName(std::string n_name){m_name=n_name;}
std::string Effect::GetName(){return m_name;}


void EffectManager::AddEffect(std::string n_name,Effect* n_effect)
{
    n_effect->SetName(n_name);
    m_effects.push_back(n_effect);
}

Effect* EffectManager::GetEffect(std::string n_name)
{
    for( auto i : m_effects )
    {
        if( i->GetName() == n_name ) return i;
    }
    return nullptr;
}

void EffectManager::ApplyAndRenderResult()
{
    Effect* tmp_last = nullptr;
    for( auto i : m_effects)
    {
        i->Apply();
        tmp_last = i;
    }
    if( tmp_last ) tmp_last->RenderResult();
}


Effect_DefaultNoEffect::Effect_DefaultNoEffect(Texture* n_inputTexture) : Effect(new Shader(GetResourcePath("effect_default.vertexshader"),GetResourcePath(""),GetResourcePath("effect_default.fragmentshader")),1.f)
{
    m_shader->Init();
    AddInputTexture("texture_screen",n_inputTexture);
}


// before applying the blur effect resize the texture
// to render faster (the details won't matter anyway)
const float EFFECT_BLUR_RESIZE_FACTOR = 0.3f;

Effect_BlurHorizontal::Effect_BlurHorizontal(Texture* n_inputTexture) : Effect(new Shader(GetResourcePath("effect_default.vertexshader"),GetResourcePath(""),GetResourcePath("effect_blurHorizontal.fragmentshader")),EFFECT_BLUR_RESIZE_FACTOR)
{
    m_shader->Init();
    AddInputTexture("texture_screen",n_inputTexture);
}

void Effect_BlurHorizontal::VSetShader()
{
    m_shader->SetFloat("screen_width",GetVideoSystem().VGetWidth()*EFFECT_BLUR_RESIZE_FACTOR);
}

Effect_BlurVertical::Effect_BlurVertical(Texture* n_inputTexture) : Effect(new Shader(GetResourcePath("effect_default.vertexshader"),GetResourcePath(""),GetResourcePath("effect_blurVertical.fragmentshader")),EFFECT_BLUR_RESIZE_FACTOR)
{
    m_shader->Init();
    AddInputTexture("texture_screen",n_inputTexture);
}

void Effect_BlurVertical::VSetShader()
{
    m_shader->SetFloat("screen_height",GetVideoSystem().VGetHeight()*EFFECT_BLUR_RESIZE_FACTOR);
}

Effect_Bloom::Effect_Bloom(Texture* n_inputTextureBlurred, Texture* n_inputTexture) : Effect(new Shader(GetResourcePath("effect_default.vertexshader"),GetResourcePath(""),GetResourcePath("effect_bloom.fragmentshader")),1.f)
{
    m_shader->Init();
    AddInputTexture("texture_screen",n_inputTexture);
    AddInputTexture("texture_blurred",n_inputTextureBlurred);
}

Effect_DOF::Effect_DOF(Texture* n_inputTexture, Texture* n_inputDepthTexture) : Effect(new Shader(GetResourcePath("effect_default.vertexshader"),GetResourcePath(""),GetResourcePath("effect_dof.fragmentshader")),0.5f)
{
    m_shader->Init();
    AddInputTexture("bgl_RenderedTexture",n_inputTexture);
    AddInputTexture("bgl_DepthTexture",n_inputDepthTexture);
}

void Effect_DOF::VSetShader()
{
    m_shader->SetFloat("bgl_RenderedTextureWidth",GetVideoSystem().VGetWidth()*0.5f);
    m_shader->SetFloat("bgl_RenderedTextureHeight",GetVideoSystem().VGetHeight()*0.5f);
}
