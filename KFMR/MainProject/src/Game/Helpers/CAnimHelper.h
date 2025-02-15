//-----------------------------------------------------------------------------
/*! \class CAnimHelper
 *  \brief Helper class to load and play game sounds
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
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
#include "CharacterActions.h"
const uint MAX_ANIMSET_SIZE = 3;
//-----------------------------------------------------------------------------
class CAnimHelper
{
	public:

		/// 
		static void Init(ECharacterAction _eActions[]);

		/// 
		static void Finish();

        /// 
		static void Play(FEHandler _hActorInst,ECharacterAction _eAction);

		/// Creates an instance of the standard actor reskined using the skin materials of the given actor.
		static FEHandler hGetReskinedActorInstance(const CFEString& _sActorSkin);

        /// Creates an actor, linked to the given actor identification name.
        static void CreateActor(const CFEString& _sActorName);

    public:

		/// Substitutes the sprite of a "named" sprite node.
		static CFESkelAnimSprite* poSubstNodeSprite(FEHandler _hAnimInst,const CFEString& _sNodeName,const CFEString& _sSprite,uint _uiAction = 0);
		
		/// Substitutes the material of a "named" mesh node.
		static CFESkelAnimMesh* poSubstNodeMesh(FEHandler _hAnimInst,const CFEString& _sNodeName,const CFEString& _sMaterial);

		/// Substitutes a given node by the provided.
		static void SubstNode(FEHandler _hAnimInst,const CFEString& _sParentNodeName,const CFEString& _sNodeName,CFESkelAnimNode* _poNode);
		
		/// Substitutes a given node by the provided.		
		static void SubstNode(CFESkelAnimNode* _poRoot,const CFEString& _sParentNodeName,const CFEString& _sNodeName,CFESkelAnimNode* _poNode);
    
        /// Adds a set of actions to the given action common set.
		static void CreateActionSet(ECharacterAction _eActions[]);

    public:

        typedef struct
        {
            uint m_uiNumAnims;
            uint m_uiAnimIdxs[MAX_ANIMSET_SIZE];

        }TActionData;

        static TActionData              m_oActionDataTable[ACTION_NUM_ACTIONS];
		static CFESkelAnimActionSet*    m_poGlobalActionSet;
		static CFESkelAnim*             m_poGlobalAnim;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
