//-----------------------------------------------------------------------------
/*! \class CGameFlowFactory
 *  \brief This class is responsible of the creation of a game flow object.
 *  \brief This file is specific of the specific game.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CGameFlowFactoryH
#define CGameFlowFactoryH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CGameFlow.h"
//-----------------------------------------------------------------------------
class CGameFlowFactory
{
	public:

		/// Creates a game flow object depending on the level.
		static CGameFlow* poCreate(uint _uiLevel,uint _uiSkill);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
