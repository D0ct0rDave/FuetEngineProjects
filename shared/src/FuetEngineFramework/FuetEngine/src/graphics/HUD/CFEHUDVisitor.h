// ----------------------------------------------------------------------------
/*! \class CHUDVisitor
 *  \brief A generic class to perform a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CHUDVisitorH
#define CHUDVisitorH
//-----------------------------------------------------------------------------
class CFEHUD;
class CFEHUDElement;
class CFEHUDObject;
class CFEHUDGroup;
class CFEHUDLabel;
class CFEHUDIcon;
class CFEHUDRect;
class CFEHUDShape;
class CFEHUDObjectAction;
class CFEHUDElementAction;
class CFEString;
//-----------------------------------------------------------------------------
class CFEHUDVisitor
{
    public:
        /// 
        virtual void Visit(CFEHUD* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDElement* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDObject* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDGroup* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDLabel* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDIcon* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDRect* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDShape* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDElementAction* _poObj) = 0;
        /// 
        virtual void Visit(CFEHUDObjectAction* _poObj) = 0;
        
        /// Retrieves the type of visitor (useful when needed to extend the basic class hierarchy).
        virtual const CFEString sGetType() = 0;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
