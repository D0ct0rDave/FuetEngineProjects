// -----------------------------------------------------------------------------
/*! \class CFEActionSleep
 *  \brief An action to perform a non active wait over the action queue.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionSleepH
#define CFEActionSleepH
// -----------------------------------------------------------------------------
#include "CFEAction.h"
// -----------------------------------------------------------------------------
class CFEActionSleep : public CFEAction
{
	public:

		/// Specific destructor for this class. If there are specific attributes it MUST not be inherited!
		virtual ~CFEActionSleep() {};

		/// Default constructor.
		CFEActionSleep() : m_rSleepTime(_0r), m_rTime(_0r) {};

		/// Initializes the action with the given parameters.
		void Init(FEReal _rTime);
		
		/// Performs an update step in the action logic.
		virtual FEBool bUpdate(FEReal _rDeltaT);
		
		/// Resets the action to its initial state.
		virtual void Reset();

		/// Forces the finish the action being played.
		virtual void ForceFinish();

		/// Retrieves the action signature.
		virtual CFEString sGetActionSignature();

	private:

		FEReal			m_rSleepTime;
		FEReal			m_rTime;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
