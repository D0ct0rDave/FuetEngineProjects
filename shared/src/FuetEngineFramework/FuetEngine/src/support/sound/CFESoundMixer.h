// ----------------------------------------------------------------------------
/*! \class CFEClass
 *  \brief A class to perform sound mixing capabilities.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESoundMixerH
#define CFESoundMixerH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
class CFESoundMixer
{
    public:

        /// Sets the volume of the given Line to a given level.
		static void SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rTime = _0r);
		
		/// Sets the user volume of the given Line to a given level.
		static void SetUserLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol);

        /// Performs a fade in from zero to the user level.
		static void FadeIn(FEReal _rTime);

        /// Performs a fade out from the user level to zero.
		static void FadeOut(FEReal _rTime);

        /// Updates the mixer state the given time step.
		static void Update(FEReal _rDeltaT);
};
// ----------------------------------------------------------------------------
#endif // CFESoundMixerH
// ----------------------------------------------------------------------------
