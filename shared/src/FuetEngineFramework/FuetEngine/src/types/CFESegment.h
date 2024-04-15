// ----------------------------------------------------------------------------
/*! \class CFESegment
 *  \brief Segment class
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESegmentH
#define CFESegmentH
// ----------------------------------------------------------------------------
#include "CFEVect2.h"
// ----------------------------------------------------------------------------
/// Class Segment
// ----------------------------------------------------------------------------
class CFESegment
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
