#include "Menu.h"
#include "Error.h"
#include "engine.h"

Menu::Menu() : ScreenLayer(ScreenLayer_Type_Menu)
{

}

void Menu::GoToNextScreenLayer(ScreenLayer* n_next)
{
	if( IsActive() == false ) return;
	RemoveThis();
	SEND_EVENT(Event_AddScreenLayer,n_next);
}

void Menu::RemoveThis()
{
	m_active = false;
	DEBUG_MESSAGE("removing menu");
	SEND_EVENT(Event_RemoveScreenLayer,this);
}


bool Menu::IsActive()
{
	return m_active;
}
