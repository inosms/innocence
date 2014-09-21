#include "inGameLogic.h"
#include "inGameLogicListener.h"

inGameLogic::inGameLogic() : GameLogic( new inGameLogicListener(this) )
{

}

void inGameLogic::Load(std::string n_levelXML)
{
	INFO("loading " << n_levelXML);

	INFO(m_state);
	m_state = GameLogicState_Loading;
	INFO(m_state);

	pugi::xml_document tmp_doc;
	pugi::xml_parse_result tmp_result = tmp_doc.load_file(GetResourcePath(n_levelXML).c_str());
	if( !tmp_result ) throw Exception(std::string("couldn't load level ") + n_levelXML + tmp_result.description());

	m_state = GameLogicState_Running;
}
