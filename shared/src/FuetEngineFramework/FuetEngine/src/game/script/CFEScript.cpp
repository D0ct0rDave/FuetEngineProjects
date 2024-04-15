// -----------------------------------------------------------------------------
/*! \class CFEScript
 *  \brief A class to store a list of actions, the script.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEScript.h"
// -----------------------------------------------------------------------------
CFEScript::~CFEScript()
{
	for (uint i=0;i<m_poActionList.size();i++)
		delete m_poActionList[i];
	
	m_poActionList.clear();
}
// -----------------------------------------------------------------------------
