// ----------------------------------------------------------------------------
/*! \class CFERect
 *  \brief Rectangle class
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
		
		/// Default constructor for the class.
        CFERect(){};

        /// Constructor for the class.
        CFERect(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY)
        {
			m_oIni.x = _rIX;
			m_oIni.y = _rIY;
			m_oEnd.x = _rFX;
			m_oEnd.y = _rFY;
        };

        /// Constructor for the class.
        CFERect(const CFEVect2& _oIni,const CFEVect2& _oEnd) : m_oIni(_oIni), m_oEnd(_oEnd) { };

		/// Translates the rectangle to a given offset.
		void Translate(const CFEVect2& _oOfs)
		{
			m_oIni += _oOfs;
			m_oEnd += _oOfs;
		}

	public:

        /// Initial coordinates of the rect
        CFEVect2 m_oIni;

        /// Final coordinates of the rect
        CFEVect2 m_oEnd;
        
        /// Retrieves the width of the rectangle
        FEReal rWidth() const { return( m_oEnd.x-m_oIni.x); };

        /// Retrieves the height of the rectangle
        FEReal rHeight() const { return( m_oEnd.y-m_oIni.y); };
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
