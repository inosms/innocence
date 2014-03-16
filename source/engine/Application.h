#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <iostream>
#include "GameLoop.h"
#include "VideoSystem.h"


class Application : public GameLoop
{
protected:
	VideoSystem* m_videoSystem;
public:

	Application();

	virtual void VUpdate();
	virtual void VRender(double n_interpolation);
	virtual void VInput();

	virtual bool VInit();
	virtual bool VExit();
};


#endif 
/* _APPLICATION_H_ */