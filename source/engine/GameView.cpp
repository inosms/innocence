#include "GameView.h"


GameView::GameView( GameView_Type n_type ) :
	m_gameViewType( n_type )
	{}

GameView_Type GameView::GetGameViewType()
{
	return m_gameViewType;
}