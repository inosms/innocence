#include "Effect.h"
#include "engine.h"

Effect::Effect(Shader* n_shader, float n_scale) :
    m_shader(n_shader)
{
    m_fbo = new FBO(n_scale);
    m_fbo->AddColorTexture();
    m_fbo->AddDepthTexture();
    m_fbo->CheckState();

    m_rect = Mesh::GetTexturedRect(1,1,0.5,0.5,nullptr,1,-1);
}

Effect::~Effect()
{
    delete m_shader;
    delete m_fbo;
    delete m_rect;
}

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
        GetVideoSystem()->VClearScreen();
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
        //GetVideoSystem()->VUpdateScreen();
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
    GetVideoSystem()->VClearScreen();
    if( tmp_last ) tmp_last->RenderResult();
    GetVideoSystem()->VUpdateScreen();
}
