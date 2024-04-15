// ----------------------------------------------------------------------------
/*! \class CHUDVisitor
 *  \brief A generic class to perform a process over all the elemnts of a HUD hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
class CFEHUDPSys;
class CFEHUDElementAction;
class CFEHUDObjectAction;
class CFEString;
//-----------------------------------------------------------------------------
class CFEHUDVisitor
{
    public:

        /// 
        virtual void Visit(CFEHUD* _poObj);

        /// 
        virtual void Visit(CFEHUDElement* _poObj);

        /// 
        virtual void Visit(CFEHUDElementAction* _poObj);

        /// 
        virtual void Visit(CFEHUDGroup* _poObj);

        /// 
        virtual void Visit(CFEHUDLabel* _poObj)
        {
			Visit((CFEHUDObject*)_poObj);
        };
        
        /// 
        virtual void Visit(CFEHUDIcon* _poObj)
        {
			Visit((CFEHUDObject*)_poObj);
        };
        
        /// 
        virtual void Visit(CFEHUDRect* _poObj)
        {
			Visit((CFEHUDObject*)_poObj);
        };
        
        /// 
        virtual void Visit(CFEHUDShape* _poObj)
        {
			Visit((CFEHUDObject*)_poObj);
        };
        
        /// 
        virtual void Visit(CFEHUDPSys* _poObj)
        {
			Visit((CFEHUDObject*)_poObj);
        };

        ///
        // virtual void Visit(CFEHUDObjectAction* _poObj)
		virtual void Visit(CFEHUDObjectAction*)
        {
			// default behavior
			// DO NOTHING        
        }

        /// 
        // virtual void Visit(CFEHUDObject* _poObj)
		virtual void Visit(CFEHUDObject*)
        {
			// default behavior
			// DO NOTHING
        }

        /// Retrieves the type of visitor (useful when needed to extend the basic class hierarchy).
        virtual const CFEString sGetType() = 0;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
