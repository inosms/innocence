#include "inGameLogic.h"
#include "inGameLogicListener.h"
#include "inGameObject.h"

inGameLogic::inGameLogic() : GameLogic( new inGameLogicListener(this) )
{

}

void inGameLogic::Load(std::string n_levelXML)
{
	INFO("loading " << n_levelXML);
	m_state = GameLogicState_Loading;

	// https://pugixml.googlecode.com/svn/tags/latest/docs/quickstart.html
	pugi::xml_document tmp_doc;
	pugi::xml_parse_result tmp_result = tmp_doc.load_file(GetResourcePath(n_levelXML).c_str());
	if( !tmp_result ) throw Exception(std::string("couldn't load level ") + n_levelXML + tmp_result.description());

	for( pugi::xml_node i : tmp_doc.children() )
	{
		int tmp_type = toInt(i.child_value("type"));
		Creator* tmp_creator = TypeToCreator(tmp_type);

		if( !tmp_creator )
		{
			ERROR_MESSAGE("No creator with type " << tmp_type << " existing!");
			continue;
		}

		GetEventManager()->SendEvent( std::shared_ptr<Event_CreateNewObject>(new Event_CreateNewObject(tmp_creator)));
	}

	m_state = GameLogicState_Running;
}
