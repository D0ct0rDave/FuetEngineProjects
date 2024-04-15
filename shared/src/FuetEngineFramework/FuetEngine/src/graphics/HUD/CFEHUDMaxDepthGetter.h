// ----------------------------------------------------------------------------
/*! \class CHUDObjectTypeIdentifier
 *  \brief A class identify the type of a CFEHUDObject
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDMaxDepthGetterH
#define CFEHUDMaxDepthGetterH
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <FuetEngine.h>
//---------------------------------------------------------------------------
class CFEHUDMaxDepthGetter : public CFEHUDVisitor
{
	public:

		CFEHUDMaxDepthGetter() :
			  m_rMaxDepth(_0r)
			, m_rDepth(_0r)
			, m_rDepthFact(_1r)
		{
		}

		static FEReal rGetMaxDepth(CFEHUDObject* _poRoot)
		{
			CFEHUDMaxDepthGetter oHMDG;
			if (_poRoot != NULL)
			{
				_poRoot->Accept(&oHMDG);
				return(oHMDG.m_rMaxDepth);
			}
			else
			{
				return(_0r);
			}
		}

		static FEReal rGetMaxDepth(CFEHUDElement* _poRoot)
		{
			CFEHUDMaxDepthGetter oHMDG;
			if (_poRoot != NULL)
			{
				_poRoot->Accept(&oHMDG);
				return(oHMDG.m_rMaxDepth);
			}
			else
			{
				return(_0r);
			}
		}

		// --------------------------------------------------------------------------
		FEReal rComputeCurrentDepth(CFEHUDObject* _poObj)
		{
			return(m_rDepth + _poObj->rGetDepth()*m_rDepthFact);
		}

		/// 
        virtual void Visit(CFEHUDObject* _poObj)
		{
			FEReal rDepth = rComputeCurrentDepth(_poObj);
			m_rMaxDepth = CFEMath::rMax(m_rMaxDepth,rDepth);
		};

        virtual void Visit(CFEHUDGroup* _poObj)
		{
			Visit((CFEHUDObject*)_poObj);

			FEReal	rOldDepth = m_rDepth;
			FEReal	rOldDepthFact = m_rDepthFact;

			m_rDepth      = rComputeCurrentDepth(_poObj);
			m_rDepthFact *= _poObj->rGetDepthFact();

				uint i = 0;
				for (uint i=0;i<_poObj->uiNumObjs();i++)
					_poObj->poGetObject(i)->Accept(this);

			m_rDepth = rOldDepth;
			m_rDepthFact = rOldDepthFact;
		};

		virtual const CFEString sGetType() { return("CFEHUDMaxDepthGetter");  };

  protected:
		
		FEReal m_rMaxDepth;

		// Accumulated depth
		FEReal m_rDepth;
		FEReal m_rDepthFact;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------