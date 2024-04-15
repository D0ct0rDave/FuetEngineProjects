// ----------------------------------------------------------------------------
/*! \class CFEScreenFX
 *  \brief A class to perform common full screen fx.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEScreenFXH
#define CFEScreenFXH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEColor.h"
// ----------------------------------------------------------------------------
class CFERenderer;
// ----------------------------------------------------------------------------
class CFEScreenFX
{
    public:

        /// Sets the background properties to fade in / out.
        static void SetBackground(FEHandler _hMat,const CFEColor& _oColor);

        /// Fades-in from the rendered graphics to stablished the background.
        static void FadeIn(FEReal _rTime);

        /// Fades-out from the stablished the background to rendered graphics.
        static void FadeOut(FEReal _rTime);

        /// Updates the current fade activity.
        static void Update(FEReal _rDeltaT);

        /// Presents the background with the current fade status.
        static void Render(CFERenderer* _poRenderer);

        /// Starts a Fades-in using current color values.
        static void ContinueFadeIn(FEReal _rTime);

        /// Starts a Fades-out using current color values.
        static void ContinueFadeOut(FEReal _rTime);

		/// Retrieves the current fade color.
		static CFEColor oGetCurrentColor()
		{
			return(m_oColor);
		}

        /// Tells whether a screen fade is being performed or not.
        static FEBool bPlaying();
        
        /// Sets the fade plane depth for rendering
        static void SetDepth(FEReal _rDepth) { m_rDepth = _rDepth; };

    protected:

        // holds the current fade material
        static FEHandler    m_hMaterial;

        // stores the fade color
        static CFEColor     m_oFadeColor;

        // stores the initial fade color
        static CFEColor     m_oIColor;

        // stores the final fade color
        static CFEColor     m_oFColor;

        // stores the final fade color
        static CFEColor     m_oColor;

        // stores the current fade time
        static FEReal       m_rTime;

        // stores the initial fade time
        static FEReal       m_rIniTime;

        // stores the Depth of the fade plane
        static FEReal       m_rDepth;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
