#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <map>
#include <string>
#include <vector>

class FBO;
class Shader;
class Texture;
class Mesh;

class Effect
{
protected:
    std::unique_ptr<FBO> m_fbo;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Mesh> m_rect;
    std::map<std::string,Texture*> m_inputTextures;
    std::string m_name = "";
public:
    // the shader is deleted with the effect
    // scale scales the size of the effect to scale times the window size
    Effect(Shader* n_shader, float n_scale = 1.f);
    virtual ~Effect();

    // adds a texture to the input;
    // the texture is NOT deleted with the effect,
    // its just a pointer to a texture somewhere (most likely a FBO)
    void AddInputTexture(std::string n_glslName, Texture* n_texture);
    void Apply();
    void RenderResult();

    // returns the first texture of the FBO
    Texture* GetResultTexture();

    // called before rendering
    virtual void VSetShader(){}

    void SetName(std::string n_name);
    std::string GetName();
};

class EffectManager
{
    std::vector<Effect*> m_effects;
public:

    void AddEffect(std::string n_name,Effect* n_effect);
    Effect* GetEffect(std::string n_name);

    // applies all effects and then renders the result
    // clears and updated the screen
    void ApplyAndRenderResult();
};

class Effect_DefaultNoEffect : public Effect
{
public:
    Effect_DefaultNoEffect(Texture* n_inputTexture);
};

class Effect_BlurHorizontal : public Effect
{
public:
    Effect_BlurHorizontal(Texture* n_inputTexture);
    virtual void VSetShader();
};

class Effect_BlurVertical : public Effect
{
public:
    Effect_BlurVertical(Texture* n_inputTexture);
    virtual void VSetShader();
};

class Effect_Bloom : public Effect
{
public:
    // adds the blurred texture to the input texture
    Effect_Bloom(Texture* n_inputTextureBlurred, Texture* n_inputTexture);
};

class Effect_DOF : public Effect
{
public:
    Effect_DOF(Texture* n_inputTexture, Texture* n_inputDepthTexture);
    virtual void VSetShader();
};

#endif
