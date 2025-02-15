// -----------------------------------------------------------------------------
/*! \class CActionPlayEntityAnim
 *  \brief This action moves an entity from an initial place to a destination in a given amount of time.
 
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CActionPlayEntityAnimH
#define CActionPlayEntityAnimH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
// -----------------------------------------------------------------------------
class CActionPlayEntityAnim : public CFEAction
{
	public:

		void Init(const CFEString& _sEntityName,const CFEString& _sAnim);

		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);

		/// Resets the action to its initial state.
		virtual void Reset();

	protected:

		CFEString	m_sEntityName;
		CFEString	m_sAnim;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------