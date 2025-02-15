// -----------------------------------------------------------------------------
/*! \class CHUDMgr
 *  \brief A class to manage HUD of the game.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CHUDMgrH
#define CHUDMgrH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "HUDActions.h"
// -----------------------------------------------------------------------------
class CFESkelAnimNode;
// -----------------------------------------------------------------------------
const uint MAX_LIVES = 5;
// -----------------------------------------------------------------------------
class CHUDMgr
{
	public:
		
		/// Initializes th HUD manager with the data specific of the given level.
		static void Init(uint _uiLevel,const CFEString& _sBoss = "");

		/// Initializes the HUD manager.		
		static void Init(CFEString _sHUDs[]);

        /// Finalizes the HUD manager an free resources.
		static void Finish();

		/// Performs an updates step over all the active entities.
		static void Update(FEReal _rDeltaT);

		/// Performs a render step of the HUD.
		static void Render(CFERenderer* _poRenderer,uint _uiHUDIdx);

		/// Updates the progress in the level of the player.
		static void SetPlayerProgress(FEReal _rProgress);

		/// Sets the face of the progress pointer to the given sprite action.
		static void SetProgressFace(uint _uiAction);

		/// Updates the energy level of the player.
		static void SetPlayerEnergy(FEReal _rEnergy);

		/// Sets the number of player lives to the given number.
		static void SetPlayerLives(uint _uiLives);

		/// Sets the energy level of a final boss.
		static void SetBossEnergy(FEReal _rEnergy);

		/// Sets the title of the current level.
		static void SetLevelTitle(const CFEString& _sLevelTitle);

		/// Sets the score of the player.
		static void SetScore(uint _uiScore);

		/// Plays the given HUD action.
		static void Play(EHUDAction _eAction);

		/// Updates the energy HUD element.
		static bool bPlayingAction(EHUDAction _eAction);
		
		/// Resets the HUD state to its initial state.
		static void Reset();		

        /// Substitute a HUD object node with a given animation node.
        static void SubstNode(CFESkelAnimNode* _poNode,const CFEString& _sParentNode,const CFEString& _sNode);

    protected:
        
        typedef struct {
        	
	        bool					m_bEnabled;
	        FEReal					m_rTime;
	        CFEHUDElementAction*	m_poAction;
	        CFEHUDObject*			m_poObject;
        	
        }THUDAction;

        static THUDAction m_oHUDActions[HA_NUM_HUD_ACTIONS];

        static FEHandler	m_hHUD[2];

        static CFEHUDElementAction* m_poPlayerEnergyAction;
        static CFEHUDElementAction* m_poBossEnergyAction;
        static CFEHUDElementAction* m_poPlayerProgressAction;
        
        static CFESkelAnimNode* m_poPointerNode;
        static CFESkelAnimSprite* m_poPointerSpriteNode;

        static CFEHUDObject* m_poLives[MAX_LIVES];
        static CFEHUDLabel* m_poLevelTitle;
        static CFEHUDLabel* m_poScore;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
