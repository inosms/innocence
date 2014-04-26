#include "ScreenLayer.h"

ScreenLayer::ScreenLayer( unsigned int n_type ) :
	m_type( n_type )
	{}

unsigned int ScreenLayer::GetType() const
{
	return m_type;
}