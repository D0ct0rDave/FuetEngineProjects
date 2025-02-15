//-----------------------------------------------------------------------------
/*! \class CLevel2BonusThrownObject
 *  \brief The class responsible of the thrown objects for the bonus 0
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CLevel2BonusThrownObjectH
#define CLevel2BonusThrownObjectH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "Game/Entity/Enemy/CThrownObject.h"
//-----------------------------------------------------------------------------
class CLevel2BonusThrownObject : public CThrownObject
{
	public:

		CLevel2BonusThrownObject();
		void Init(bool _bIsGood, int _iIndex);

		/// Accept procedure for processing functionality over this entity
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

		// 
		virtual void Update(FEReal _rDeltaT);

		/// Get the kind of object
		bool bIsGood() { return m_bIsGood; }
	
	protected:

		virtual void Kill();

	private:

		bool m_bIsGood;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

