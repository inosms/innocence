#ifndef _GAMEVIEW_H_
#define _GAMEVIEW_H_

#include <vector>
#include "ScreenLayer.h"
#include "Process.h"
#include "ScreenLayer_Scene.h"
#include "VideoSystem.h"
#include "EventListener.h"

enum GameView_Type
{
	GameView_Type_AI,
	GameView_Type_Human
};

class GameView
{
protected:
	GameView_Type m_gameViewType;

	// a unique identifier for a game view
	unsigned int m_id;

	// this gives the gameViews an id
	unsigned int GenerateGameViewID();
public:

	GameView( GameView_Type n_type );

	GameView_Type GetGameViewType();
	unsigned int GetID();

	virtual void VRender(double n_interpolation) = 0;
	virtual void VUpdate() = 0;	

	// this is called when being attached
	virtual void VInit() = 0;
};

class GameView_Human : public GameView
{
protected:
	std::vector<ScreenLayer*> m_screenLayers;

	// this is just another pointer to the scene
	// for quick access
	// the scene is also in the m_screenLayers List!
	ScreenLayer_Scene* m_scene;
	EventListener* m_listener;
public:

	GameView_Human(EventListener* n_listener);
	virtual void VRender( double n_interpolation );
	virtual void VUpdate();
	virtual void VInit();

	void AddScreenLayer( ScreenLayer* n_layer );
	ScreenLayer_Scene* GetScene();
};

// this is the process which is started alongside with
// GameView_Human as it manages its game own game loop
class HumanView_Process : public Thread_Process
{
	GameView_Human* m_view;
	unsigned int m_lastTickTime;
	unsigned int m_tickDelayTime;
public:
	HumanView_Process(GameView_Human* n_view);
	virtual void VThreadedMethod();
	virtual void VInitialize();
};

#endif