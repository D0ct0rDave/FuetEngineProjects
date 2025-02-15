// ----------------------------------------------------------------------------
/*! \class CFEKFBFunc
 *  \brief Key Frame Based Function
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEKFBFunc.h"
// ----------------------------------------------------------------------------
static void Test()
{
	CFEKFBFunc<FEReal> oR;
	oR.InsertKeyFrame( 0, 1, KFLF_RAND);
	oR.poGetKeyFrameAtTime(0);
	FEReal r = oR.oGetValue(0);
}
// ----------------------------------------------------------------------------
