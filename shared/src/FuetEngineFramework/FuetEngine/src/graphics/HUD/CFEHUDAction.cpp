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
#include "CFEHUDAction.h"
//-----------------------------------------------------------------------------
CFEHUDObjectAction::~CFEHUDObjectAction()
{
	m_oPosFunc.Reset();
	m_oScaleFunc.Reset();
	m_oColorFunc.Reset();
	m_rAngleFunc.Reset();
}
//-----------------------------------------------------------------------------
CFEHUDElementAction::~CFEHUDElementAction()
{
	///
	for (uint a=0;a<m_oObjectActions.size();a++)
		delete m_oObjectActions[a];

	/// 
	m_oObjectActions.clear();
}
//-----------------------------------------------------------------------------

