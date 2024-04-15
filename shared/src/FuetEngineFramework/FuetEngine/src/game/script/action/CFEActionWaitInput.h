// -----------------------------------------------------------------------------
/*! \class CFEActionWaitInput
 *  \brief An action to perform a non active wait over the action queue.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionWaitInputH
#define CFEActionWaitInputH
// -----------------------------------------------------------------------------
#include "CFEAction.h"
// -----------------------------------------------------------------------------
class CFEActionWaitInput : public CFEAction
{
	public:

		/// Specific destructor for this class. If there are specific attributes it MUST not be inherited!
		virtual ~CFEActionWaitInput() {};

		/// Default constructor.
		CFEActionWaitInput(){};

		/// Initializes the action with the given parameters.
		void Init();
		
		/// Performs an update step in the action logic.
		virtual FEBool bUpdate(FEReal _rDeltaT);
		
		/// Resets the action to its initial state.
		virtual void Reset();

		/// Forces the finish the action being played.
		virtual void ForceFinish();

		/// Retrieves the action signature.
		virtual CFEString sGetActionSignature();

	private:

		FEBool m_bExit;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
