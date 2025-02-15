//-----------------------------------------------------------------------------
/*! \class CAnimHelper
 *  \brief Helper class to load and play game sounds
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CAnimHelperH
#define CAnimHelperH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CFESkelAnimSprite;
//-----------------------------------------------------------------------------
enum EGameAnim
{
	ANIM_ACTOR_WALK,
	
	ANIM_ACTOR_IDLE,
	ANIM_ACTOR_KICK,
	ANIM_ACTOR_PUNCH,

	ANIM_ACTOR_CROUCH_IDLE,
	ANIM_ACTOR_CROUCH_KICK,
	ANIM_ACTOR_CROUCH_PUNCH,

	ANIM_ACTOR_JUMP_KICK,

	ANIM_ACTOR_PAIN,

	ANIM_ACTOR_DEATH,
};
//-----------------------------------------------------------------------------
class CAnimHelper
{
	public:

		/// 
		static void Init(uint _uiLevel);

		/// 
		static void Finish();

		static void Play(FEHandler _hActorInst,EGameAnim _eAnim);
		
		/// Substitutes the sprite of a "named" sprite node.
		static CFESkelAnimSprite* poSubstNode(FEHandler _hAnimInst,const CFEString& _sNodeName,const CFEString& _sSprite,uint _uiAction = 0);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
