//-----------------------------------------------------------------------------
/*! \class CSoundHelper
 *  \brief Helper class to load and play game sounds
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CSoundHelperH
#define CSoundHelperH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
enum EGameSound
{
	SND_PLAYER_ATTACK,
	SND_PLAYER_JUMP_ATTACK,
	SND_PLAYER_PAIN,
	SND_PLAYER_HIT,
	SND_PLAYER_DEATH,
	
	SND_ENEMY_ATTACK,
	SND_ENEMY_PAIN,
	SND_ENEMY_HIT,
	SND_ENEMY_DEATH,
	
	SND_THROWN,
	SND_OBJECT_HIT,
	
	SND_CHINOCUDEIRO_ATTACK,
};
//-----------------------------------------------------------------------------
class CSoundHelper
{
	public:

		/// 
		static void Init(uint _uiLevel);

		/// 
		static void Finish();
		
		
		static void Play(EGameSound _eSound, FEReal _rVol = 1.0f, FEReal _rPan = 0.5f,bool _bLoop = false);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
