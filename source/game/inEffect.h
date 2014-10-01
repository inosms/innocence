#ifndef _INEFFECT_H_
#define _INEFFECT_H_

#include "../engine/engine.h"

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
