#include "inMenu.h"


Menu_MainMenu::Menu_MainMenu() : Menu()
{

}

void Menu_MainMenu::VUpdate()
{
	std::cout << "mainmenu" << std::endl;
}
void Menu_MainMenu::VRender( double n_interpolation )
{

}
bool Menu_MainMenu::VOnEvent(Event_Input& n_event)
{
	std::cout << "EVENT ASDF" << std::endl;

	if( n_event.GetType() == Event_Type_Input_Mousebutton_Down )
	{
		std::cout << "MOUSE BUTTON DOWN! GO TO NEXT SCREEN!" << std::endl;
		GoToNextScreenLayer(new Menu_Test());	
	}
	return false;
}


Menu_Test::Menu_Test() : Menu()
{

}

void Menu_Test::VUpdate()
{
	std::cout << "testmenu" << std::endl;
}
void Menu_Test::VRender( double n_interpolation )
{

}
bool Menu_Test::VOnEvent(Event_Input& n_event)
{
	return false;
}