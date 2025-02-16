// ----------------------------------------------------------------------------
/*! \class CFEHUDInstancer
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDInstancerH
#define CFEHUDInstancerH
//-----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDInstancer : public CFEHUDVisitor
{
    public:
        CFEHUD* poCreateInstance(CFEHUD* _poHUDModel);

        /// 
        virtual void Visit(CFEHUD* _poObj);
        /// 
        virtual void Visit(CFEHUDElement* _poObj);
        /// 
        virtual void Visit(CFEHUDObject* _poObj);
        /// 
        virtual void Visit(CFEHUDGroup* _poObj);
        /// 
        virtual void Visit(CFEHUDLabel* _poObj);
        /// 
        virtual void Visit(CFEHUDIcon* _poObj);
        /// 
        virtual void Visit(CFEHUDRect* _poObj);
        /// 
        virtual void Visit(CFEHUDElementAction* _poObj);
        /// 
        virtual void Visit(CFEHUDObjectAction* _poObj);

};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------