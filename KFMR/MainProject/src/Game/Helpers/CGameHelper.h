//-----------------------------------------------------------------------------
/*! \class CGameHelperH
 *  \brief A class which provides helping functionality to the game code.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CGameHelperH
#define CGameHelperH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CGameHelper
{
	public:
	    static bool bVisibleOnScreen(const CFEVect2& _oPos,FEReal _rObjectWidth = _0r) ;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
