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
    FBO* m_fbo;
    Shader* m_shader;
    Mesh* m_rect;
    std::map<std::string,Texture*> m_inputTextures;
    std::string m_name = "";
public:
    // the shader is deleted with the effect
    Effect(Shader* n_shader);
    ~Effect();

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

#endif
