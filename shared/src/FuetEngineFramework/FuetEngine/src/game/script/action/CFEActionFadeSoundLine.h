// -----------------------------------------------------------------------------
/*! \class CFEActionFadeSoundLine
 *  \brief Fades the sound in a sound line.
  *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par WordsUp License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionFadeSoundLineH
#define CFEActionFadeSoundLineH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
// -----------------------------------------------------------------------------
class CFEActionFadeSoundLine : public CFEAction
{
	public:

		///
		void Init(EFESoundMixerLine _eMixLine,FEReal _rVol,FEReal _rFadeTime,FEBool _bWait);

		/// Performs an update step in the action logic.
		virtual FEBool bUpdate(FEReal _rDeltaT);

		/// Resets the action to its initial state.
		virtual void Reset();

		/// Retrieves the action signature.
		virtual CFEString sGetActionSignature();

	protected:

		EFESoundMixerLine	m_eMixLine;
		FEReal				m_rVol;
		FEReal				m_rFadeTime;
		FEBool				m_bWait;
		FEReal				m_rTime;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------

