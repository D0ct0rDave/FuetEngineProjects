// -----------------------------------------------------------------------------
/*! \class CActionStopBGM
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CActionStopBGM.h"
#include "Game/Helpers/CSoundHelper.h"
// -----------------------------------------------------------------------------
bool CActionStopBGM::bUpdate(FEReal _rDeltaT)
{
    CSoundHelper::StopBGM();
    return(false);
}
// -----------------------------------------------------------------------------
