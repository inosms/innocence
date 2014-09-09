#include "Menu.h"
#include "Error.h"
#include "engine.h"

Menu::Menu() : ScreenLayer(ScreenLayer_Type_Menu)
{

}

void Menu::GoToNextScreenLayer(ScreenLayer* n_next)
{
	RemoveThis();
	GetEventManager()->SendEvent( std::shared_ptr<Event_AddScreenLayer>( new Event_AddScreenLayer(n_next)));
	}

void Menu::RemoveThis()
{
	DEBUG_MESSAGE("removing menu");
	GetEventManager()->SendEvent( std::shared_ptr<Event_RemoveScreenLayer>( new Event_RemoveScreenLayer(this)));
}