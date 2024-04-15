// -----------------------------------------------------------------------------
/*! \class CFEActionStopSound
 *  \brief An action to play sounds
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionStopSoundH
#define CFEActionStopSoundH
// -----------------------------------------------------------------------------
#include "CFEAction.h"
// -----------------------------------------------------------------------------
class CFEActionStopSound : public CFEAction
{
	public:

		/// Specific destructor for this class. If there are specific attributes it MUST not be inherited!
		virtual ~CFEActionStopSound() {};

		/// Initializes the action with the given sound to be played.
		void Init(FEHandler _hSoundResource);

		/// Performs an update step in the action logic.
		virtual FEBool bUpdate(FEReal _rDeltaT);

		/// Resets the action to its initial state.
		virtual void Reset();

		/// Retrieves the action signature.
		virtual CFEString sGetActionSignature();

	protected:

		FEHandler			m_hSoundResource;
};

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
