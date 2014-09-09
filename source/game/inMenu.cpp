#include "inMenu.h"


Menu_MainMenu::Menu_MainMenu() : Menu()
{

}

void Menu_MainMenu::VUpdate()
{
}
void Menu_MainMenu::VRender( double n_interpolation )
{

}
bool Menu_MainMenu::VOnEvent(Event_Input& n_event)
{
	std::cout << "EVENT ASDF" << std::endl;
	return false;
}


Menu_Test::Menu_Test() : Menu()
{

}

void Menu_Test::VUpdate()
{
	std::cout << "here" << std::endl;
}
void Menu_Test::VRender( double n_interpolation )
{

}
bool Menu_Test::VOnEvent(Event_Input& n_event)
{
	return false;
}