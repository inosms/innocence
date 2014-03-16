#ifndef _VIDEOSYSTEM_H_
#define _VIDEOSYSTEM_H_

class VideoSystem
{
public: 

	virtual bool VInit() = 0;

	virtual bool VExit() = 0;

	virtual void VClearScreen() = 0;
	virtual void VUpdateScreen() = 0;
	
	// TODO  
};

#endif /* _VIDEOSYSTEM_H_ */