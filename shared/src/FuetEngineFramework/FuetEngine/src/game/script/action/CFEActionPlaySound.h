// -----------------------------------------------------------------------------
/*! \class CFEActionPlaySound
 *  \brief An action to play sounds
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
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
		
		/// Initializes the action with the given sound to be played.
		void Init(FEHandler _hSound,EFESoundMixerLine _eMixLine,FEReal _rVol,FEReal _rPan,bool _bLoop,bool _bWait);

		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);

		/// Resets the action to its initial state.
		virtual void Reset();
	
	protected:

		FEHandler			m_hSound;
		EFESoundMixerLine	m_eMixLine;
		FEReal				m_rVol;
		FEReal				m_rPan;
		bool				m_bWait;
};

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------