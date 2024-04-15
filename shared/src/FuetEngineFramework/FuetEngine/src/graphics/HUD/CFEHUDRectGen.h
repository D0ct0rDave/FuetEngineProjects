// ----------------------------------------------------------------------------
/*! \class CFEHUDRectGen
 *  \brief A class to save a HUD layout.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDRectGenH
#define CFEHUDRectGenH
//-----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
#include "types/CFERect.h"
#include "types/CFEMatrix.h"
#include "Types/CFEString.h"
//-----------------------------------------------------------------------------
class CFEHUDRectGen : public CFEHUDVisitor
{
    public:
		CFEHUDRectGen(FEPointer _poTarget);

        static CFERect oGetRect(CFEHUDGroup* _poParent,CFEHUDObject* _poObj);
        
        static CFERect oGetRect(CFEHUD* _poObj);
        
        // Retrieves the result rectangle in case we need to use
        // the generator explicitly.
        CFERect oGetRect()
        {
			return(m_oRect);
        }

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
        virtual void Visit(CFEHUDPSys* _poObj);


        virtual void Visit(CFEHUD* _poObj);
        /// 
        virtual void Visit(CFEHUDElement* _poObj);
        /// 
        virtual void Visit(CFEHUDElementAction* _poObj);
        ///        
        virtual void Visit(CFEHUDObjectAction* _poObj);

		void Init();

		virtual const CFEString sGetType() { return("CFEHUDRectGen"); };

    protected:
    
		void GenericRectGen(CFEHUDObject* _poObj);

        FEBool m_bTargetFound;
        
        FEPointer	m_poTarget;
        //
        CFERect		m_oRect;
        
        CFEMatrix	m_oTransf;

		FEReal		m_rDepth;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
