// ----------------------------------------------------------------------------
/*! \class CFEHUDUpdater
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDUpdater.h"

#include "CFEHUDAction.h"

#include "CFEHUD.h"
#include "CFEHUDelement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
//-----------------------------------------------------------------------------
FEReal CFEHUDUpdater::m_rTime = _0r;
//-----------------------------------------------------------------------------
void CFEHUDUpdater::Update(FEHandler _hHUDInstance,FEReal _rDeltaT)
{
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::Process(CFEHUDElementAction* _poObj)
{
    for (uint i=0;i<_poObj->uiNumActions();i++)
        Process( _poObj->poGetAction(i) );
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::Process(CFEHUDObjectAction* _poObj)
{
    _poObj->oGetValue(m_rTime);
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::SetTime(FEReal _rTime)
{
    m_rTime = _rTime;
}
//-----------------------------------------------------------------------------
