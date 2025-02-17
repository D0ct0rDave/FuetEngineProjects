//-----------------------------------------------------------------------------
/*! \class CProfile
 *  \brief The class that stores the current player state.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CProfileH
#define CProfileH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CProfile
{
	public:
	    
	    //
	    CProfile() { Init(); };

		/// 
		void Init()
		{
		    m_uiCurLevel = 0x1a;
		    m_uiScore = 0;
		    m_uiCurSkill = 0;
		}

	public:

        /// The skill selected for the player.
		uint	m_uiCurSkill;

        /// The current level of the player.
		uint	m_uiCurLevel;
    
        /// The player score
		uint	m_uiScore;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
