#include "inGameLogic.h"
#include "inGameLogicListener.h"
#include "inGameObject.h"
#include "inEvent.h"

inGameLogic::inGameLogic() : GameLogic( new inGameLogicListener(this) )
{
	GetEventManager().AddEventListener(m_listener,inEvent_Type_RequestPlayerMove);
	GetEventManager().AddEventListener(m_listener,inEvent_Type_RequestPlayerJump);
	GetEventManager().AddEventListener(m_listener,inEvent_Type_RequestPlayerStop);
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
		inCreator* tmp_creator = TypeToCreator(tmp_type);
		tmp_creator->VLoad(i);

		if( !tmp_creator )
		{
			ERROR_MESSAGE("No creator with type " << tmp_type << " existing!");
			continue;
		}

		SEND_EVENT(Event_CreateNewObject,tmp_creator);
	}

	// send this as an event, as a result this is processed
	// after all the Create Events
	SEND_EVENT(Event_SetGameState,GameLogicState_Running);

	// TODO: extra state and event! otherwise its not in order...
	m_levelStarted = true;
}


GameObject* inGameLogic::GetPlayer()
{
	for( auto i : m_objects )
	{
		// FIXME: this only for testing
		if( dynamic_cast<GameObject_PlayerTest*>(i.get()) )
			return i.get();
	}
	return nullptr;
}
