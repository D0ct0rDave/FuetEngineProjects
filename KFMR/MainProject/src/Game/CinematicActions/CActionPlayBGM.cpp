// -----------------------------------------------------------------------------
/*! \class CActionPlayBGM
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CActionPlayBGM.h"
#include "Game/Helpers/CSoundHelper.h"
// -----------------------------------------------------------------------------
bool CActionPlayBGM::bUpdate(FEReal _rDeltaT)
{
    CSoundHelper::PlayBGM();
    return(false);
}
// -----------------------------------------------------------------------------
