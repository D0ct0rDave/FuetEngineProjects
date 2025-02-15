// ----------------------------------------------------------------------------
/*! \class CFERect
 *  \brief Rectangle class
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFERectH
#define CFERectH
// ----------------------------------------------------------------------------
#include "CFEVect2.h"
// ----------------------------------------------------------------------------
/// Class Rect
// ----------------------------------------------------------------------------
class CFERect
{
    public:
        /// Initial coordinates of the rect
        CFEVect2 m_oIni;

        /// Final coordinates of the rect
        CFEVect2 m_oEnd;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
