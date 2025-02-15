// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnim.h"
//-----------------------------------------------------------------------------
CFESkelAnim::~CFESkelAnim()
{
	delete m_poActionSet;
	
	delete m_poAnimNode;

	/*
	// already deleted with "delete m_poAnimNode"
	for (uint i=0;i<m_oNodeTab.size();i++)
		delete m_oNodeTab[i];
	*/
}
//-----------------------------------------------------------------------------
CFESkelAnimActionSet::~CFESkelAnimActionSet()
{
	for (uint i=0;i<m_oActions.size();i++)
		delete m_oActions[i];
}
//-----------------------------------------------------------------------------
CFESkelAnimAction::~CFESkelAnimAction()
{
	for (uint i=0;i<m_oNodeActions.size();i++)
		delete m_oNodeActions[i];
}
//-----------------------------------------------------------------------------

