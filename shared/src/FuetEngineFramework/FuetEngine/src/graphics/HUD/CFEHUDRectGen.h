// ----------------------------------------------------------------------------
/*! \class CFEHUDRectGen
 *  \brief A class to save a HUD layout.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDRectGenH
#define CFEHUDRectGenH
//-----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
#include "types/CFERect.h"
#include "types/CFEMatrix.h"
//-----------------------------------------------------------------------------
class CFEHUDRectGen : public CFEHUDVisitor
{
    public:
        CFERect oGetRect(CFEHUDGroup* _poParent,CFEHUDObject* _poObj);

        virtual void Visit(CFEHUDObject* _poObj);
        /// 
        virtual void Visit(CFEHUDGroup* _poObj);
        /// 
        virtual void Visit(CFEHUDLabel* _poObj);
        /// 
        virtual void Visit(CFEHUDIcon* _poObj);
        /// 
        virtual void Visit(CFEHUDRect* _poObj);


        virtual void Visit(CFEHUD* _poObj);
        /// 
        virtual void Visit(CFEHUDElement* _poObj);
        /// 
        virtual void Visit(CFEHUDElementAction* _poObj);
        ///        
        virtual void Visit(CFEHUDObjectAction* _poObj);

    protected:
        
        bool m_bTargetFound;
        
        CFEHUDObject* m_poTarget;
        //
        CFERect m_oRect;
        
        CFEMatrix m_oTransf;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------