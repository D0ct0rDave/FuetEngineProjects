// ----------------------------------------------------------------------------
/*! \class CFEHUDRenderer
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDRendererH
#define CFEHUDRendererH
//-----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
#include "FEBasicTypes.h"
#include "Types/CFEColor.h"
#include "Types/CFEString.h"
//-----------------------------------------------------------------------------
class CFERenderer;
//-----------------------------------------------------------------------------
class CFEHUDRenderer : public CFEHUDVisitor
{
    public:

        CFEHUDRenderer()
        {
			m_poRenderer = NULL;
			m_oModColor = CFEColor::WHITE();
			m_rDepth    = _0r;
        }

        void Render(FEHandler _hHUDInstance,CFERenderer* _poRenderer);

        /// Sets the renderer for this class. This way visit functions can be called externally.
        void SetRenderer(CFERenderer* _poRenderer)
        {
            m_poRenderer = _poRenderer;
        }

        /// Retrieves the renderer used on this class.
        CFERenderer* poGetRenderer()
        {
            return(m_poRenderer);
        }

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
        virtual void Visit(CFEHUDShape* _poObj);
        /// 
        virtual void Visit(CFEHUDElementAction* _poObj);
        /// 
        virtual void Visit(CFEHUDObjectAction* _poObj);
		
		virtual const CFEString sGetType() { return("CFEHUDRenderer"); };

    protected:

		// The renderer to use when rendering.
        CFERenderer*	m_poRenderer;
        
        // Accumulated modulation color to render hud objects.
        CFEColor		m_oModColor;
        
        // Accumulated depth
        FEReal			m_rDepth;
        
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------