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
	GetEventManager()->SendEvent( std::shared_ptr<Event_AddScreenLayer>( new Event_AddScreenLayer(n_next)));
	}

void Menu::RemoveThis()
{
	m_active = false;
	DEBUG_MESSAGE("removing menu");
	GetEventManager()->SendEvent( std::shared_ptr<Event_RemoveScreenLayer>( new Event_RemoveScreenLayer(this)));
}


bool Menu::IsActive()
{
	return m_active;
}