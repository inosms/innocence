#ifndef _GAMEVIEW_H_
#define _GAMEVIEW_H_

enum GameView_Type
{
	GameView_Type_AI,
	GameView_Type_Human
};

class GameView
{
protected:
	GameView_Type m_gameViewType;

public:

	GameView( GameView_Type n_type );

	GameView_Type GetGameViewType();

	virtual void VRender(double n_interpolation) = 0;
	virtual void VUpdate() = 0;	
};

#endif