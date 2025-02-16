// ----------------------------------------------------------------------------
/*! \class CFESoundMixer
 *  \brief CFESoundMixer Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version _1r
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESoundMixer.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
typedef enum
{
    MS_NONE,

    MS_IDLE,
    MS_FADEIN,
    MS_FADEOUT,

    MS_NUMSTATES

}EMixState;
// ----------------------------------------------------------------------------
static FEReal       gsrUserLines[SML_NUM_LINES] = { _1r, _1r, _1r, _1r, _1r};
static FEReal       gsrFadeFact = _1r;
static FEReal       gsrFadeStep = _1r;
static EMixState    gseMixState = MS_IDLE;
// ----------------------------------------------------------------------------
void CFESoundMixer::SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol)
{
    gsrUserLines[_eSoundLine] = _rVol;

    // Update the system lines
    CFESystem::Sound::SetLineLevel(_eSoundLine,gsrUserLines[_eSoundLine]*gsrFadeFact);
}
// ----------------------------------------------------------------------------
void CFESoundMixer::FadeIn(FEReal _rTime)
{
    gsrFadeFact     = _0r;
    gsrFadeStep     = _1r / _rTime;
    gseMixState     = MS_FADEIN;
}
// ----------------------------------------------------------------------------
void CFESoundMixer::FadeOut(FEReal _rTime)
{
    gsrFadeFact = _1r;
    gsrFadeStep = -_1r / _rTime;
    gseMixState = MS_FADEOUT;
}
// ----------------------------------------------------------------------------
void CFESoundMixer::Update(FEReal _rDeltaT)
{
    switch (gseMixState)
    {
        case MS_FADEIN:
        {
            gsrFadeFact += gsrFadeStep*_rDeltaT;
            if (gsrFadeFact >= _1r)
            {
                gsrFadeFact = _1r;
                gseMixState = MS_IDLE;
            }

            ///
            for (uint i=0;i<SML_NUM_LINES;i++)
                CFESystem::Sound::SetLineLevel((EFESoundMixerLine)i,gsrUserLines[i]*gsrFadeFact);
        }
        break;

        case MS_FADEOUT:
        {
            gsrFadeFact += gsrFadeStep*_rDeltaT;
            if (gsrFadeFact <= _0r)
            {
                gsrFadeFact = _0r;
                gseMixState = MS_IDLE;
            }

            ///
            for (uint i=0;i<SML_NUM_LINES;i++)
                CFESystem::Sound::SetLineLevel((EFESoundMixerLine)i,gsrUserLines[i]*gsrFadeFact);
        }
        break;
    }
}
// ----------------------------------------------------------------------------
