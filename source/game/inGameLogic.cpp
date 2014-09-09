#include "inGameLogic.h"
#include "inGameLogicListener.h"

inGameLogic::inGameLogic() : GameLogic( new inGameLogicListener(this) )
{

}