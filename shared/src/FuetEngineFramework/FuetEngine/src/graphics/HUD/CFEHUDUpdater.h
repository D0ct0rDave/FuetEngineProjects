// ----------------------------------------------------------------------------
/*! \class CFEHUDUpdater
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDUpdaterH
#define CFEHUDUpdaterH
//-----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
#include "FEBasicTypes.h"
//-----------------------------------------------------------------------------
class CFEHUDElementAction;
class CFEHUDObjectAction;
//-----------------------------------------------------------------------------
class CFEHUDUpdater /* : public CFEHUDVisitor*/
{
    public:
        static void Update(FEHandler _hHUDInstance,FEReal _rDeltaT);

        /// 
        static void Process(CFEHUDElementAction* _poObj);

        /// 
        static void Process(CFEHUDObjectAction* _poObj);

        ///
        static void SetTime(FEReal _rTime);

    protected:
        static FEReal  m_rTime;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------