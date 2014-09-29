#ifndef _INHUMANVIEW_H_
#define _INHUMANVIEW_H_

#include "../engine/engine.h"

class inHumanView : public GameView_Human
{
	EffectManager m_effectManager;
public:

	inHumanView();
	virtual void VInit();
	virtual void VRender(double n_interpolation);
};

#endif
