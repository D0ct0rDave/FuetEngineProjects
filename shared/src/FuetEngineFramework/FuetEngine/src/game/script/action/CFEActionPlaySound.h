// -----------------------------------------------------------------------------
/*! \class CFEActionPlaySound
 *  \brief An action to play sounds
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionPlaySoundH
#define CFEActionPlaySoundH
// -----------------------------------------------------------------------------
#include "CFEAction.h"
#include "FEBasicTypes.h"
#include "FEEnums.h"
// -----------------------------------------------------------------------------
class CFEActionPlaySound : public CFEAction
{
	public:

		/// Specific destructor for this class. If there are specific attributes it MUST not be inherited!
		virtual ~CFEActionPlaySound() {};
	
		/// Initializes the action with the given sound to be played.
		void Init(FEHandler _hSoundResource,EFESoundMixerLine _eMixLine,FEReal _rVol,FEReal _rPan,FEBool _bLoop,FEBool _bWait);

		/// Performs an update step in the action logic.
		virtual FEBool bUpdate(FEReal _rDeltaT);

		/// Resets the action to its initial state.
		virtual void Reset();

		/// Retrieves the action signature.
		virtual CFEString sGetActionSignature();

	protected:

		FEHandler			m_hSoundResource;
		EFESoundMixerLine	m_eMixLine;
		FEReal				m_rVol;
		FEReal				m_rPan;
		FEBool				m_bWait;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
