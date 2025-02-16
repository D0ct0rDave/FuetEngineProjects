// -----------------------------------------------------------------------------
/*! \class CEntityMgr
 *  \brief A class to manage all the entities of the system.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CHUDMgr.h"
#include "CHUDActorNode.h"

#include "Game/CGameGlobals.h"

// -----------------------------------------------------------------------------
CHUDMgr::THUDAction     CHUDMgr::m_oHUDActions[HA_NUM_HUD_ACTIONS];
FEHandler	            CHUDMgr::m_hHUD[2];

CFEHUDElementAction*    CHUDMgr::m_poPlayerEnergyAction       = NULL;
CFEHUDElementAction*    CHUDMgr::m_poBossEnergyAction         = NULL;
CFEHUDElementAction*    CHUDMgr::m_poPlayerProgressAction     = NULL;

CFESkelAnimNode*        CHUDMgr::m_poPointerNode = NULL;
CFESkelAnimSprite*      CHUDMgr::m_poPointerSpriteNode = NULL;

CFEHUDObject*           CHUDMgr::m_poLives[MAX_LIVES] = {NULL,NULL,NULL,NULL,NULL};
CFEHUDLabel*            CHUDMgr::m_poLevelTitle = NULL;
CFEHUDLabel*            CHUDMgr::m_poScore = NULL;
// -----------------------------------------------------------------------------
void CHUDMgr::SubstNode(CFESkelAnimNode* _poNode,const CFEString& _sParentNode,const CFEString& _sNode)
{
    // poNode,"LevelProgressGroup","ProgressPointer");
    for (uint i=0;i<2;i++)
    {
        /// Retrieves the element that matches with the specified name.
        CFEHUDElement* poElem = NULL;
        CFEHUDGroup* poGroup  = NULL;
        
        CFEHUDElemLocator oElemLocator;
        poElem = oElemLocator.poLocateHUDElement((CFEHUD*)m_hHUD[i],_sParentNode);

        if (poElem != NULL)
        {
            poGroup = (CFEHUDGroup*)oElemLocator.poLocateHUDObject(poElem,_sParentNode);

            if ((poElem!=NULL) && (poGroup!=NULL))
            {
                CFEHUDObject* poOldObj = NULL;
                CHUDActorNode* poNewNode = new CHUDActorNode(_sNode);
				poNewNode->SetActorNode(_poNode);

                // Locate object.
                for (uint j=0;j<poGroup->uiNumObjs();j++)
                {
                    if (poGroup->poGetObject(j)->sGetName() |= _sNode)
                    {
                        poOldObj = poGroup->poGetObject(j);

                        poNewNode->SetPos(poOldObj->oGetPos());
                        poNewNode->SetScale(poOldObj->oGetScale());
                        poNewNode->SetAngle(poOldObj->rGetAngle());
                        poNewNode->SetColor(poOldObj->oGetColor());

                        // Substitue node
                        poGroup->SetObject(j,poNewNode);
                        break;
                    }
                }

                // Setup actions using the old object.
                for (uint j=0;j<poElem->uiNumActions();j++)
                {
                    CFEHUDElementAction* poAction = poElem->poGetAction(j);

                    for (uint k=0;k<poAction->uiNumActions();k++)
                    {
                        if (poAction->poGetAction(k)->poGetHUDObject() == poOldObj)
                            poAction->poGetAction(k)->SetHUDObject( poNewNode );
                    }
                }
            }
        }
    }
}
// -----------------------------------------------------------------------------
void CHUDMgr::Init(uint _uiLevel,const CFEString& _sBoss)
{
    #ifdef DUAL_SCREEN

        switch (_uiLevel & 0x0f)
        {
            // Level Part I
            case 0x0a:
            {
                CFEString sHUDs[] = 
                {
                    "data/HUD/HUD_up",
                    "data/HUD/HUD_low",
                    ""
                };

                CHUDMgr::Init(sHUDs);

                // substitute the pointer node.
                CFESkelAnimNode* m_poPointerNode = CFESkelAnimNodeInstancer::poCreateInstance( CFESkelAnimLoader::poLoadNode("data/actors/player/face") );
                CHUDMgr::SubstNode(m_poPointerNode,"LevelProgressGroup","ProgressPointer");
                m_poPointerNode->SetScale(CFEVect2(0.75,0.75));
                m_poPointerSpriteNode = (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode(m_poPointerNode,"face");
                CFESpriteInstMgr::Enable(m_poPointerSpriteNode->hGetSprite());
            }
            break;
        
            // Level Part II with boss
            case 0x0b:
            {
                CFEString sHUDs[] = 
                {
                    "data/HUD/HUD_up",
                    "data/HUD/HUD_low",
                    ""
                };
                
                CHUDMgr::Init(sHUDs);

                // substitute the pointer node.
                CFESkelAnimNode* m_poPointerNode = CFESkelAnimNodeInstancer::poCreateInstance( CFESkelAnimLoader::poLoadNode("data/actors/player/face") );
                CHUDMgr::SubstNode(m_poPointerNode,"LevelProgressGroup","ProgressPointer");
                m_poPointerNode->SetScale(CFEVect2(0.75,0.75));
                m_poPointerNode->SetPos( CFEVect2(_0r,-5.0f));
                m_poPointerSpriteNode = (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode(m_poPointerNode,"face");
                CFESpriteInstMgr::Enable(m_poPointerSpriteNode->hGetSprite());
                m_poPointerSpriteNode->SetAction(0);

                // substitute the player face in the "versus" screen.
                CFESkelAnimNode* poPlayerFace = CFESkelAnimNodeInstancer::poCreateInstance( CFESkelAnimLoader::poLoadNode("data/actors/player/face") );
                CHUDMgr::SubstNode(poPlayerFace,"Versus_Player","PlayerAvatar");
                
                CFEString sBossFace = CFEString("data/actors/") + _sBoss + CFEString("/face");
                CFESkelAnimNode* poBossFace = CFESkelAnimNodeInstancer::poCreateInstance( CFESkelAnimLoader::poLoadNode(sBossFace) );
                CHUDMgr::SubstNode(poBossFace,"Versus_Boss","BossAvatar");
            }
            break;

            // Bonus Level.
            case 0x0c:
            {
                CFEString sHUDs[] = 
                {
                    "data/HUD/HUD_up",
                    "data/HUD/HUD_low",
                    ""
                };
                
                CHUDMgr::Init(sHUDs);
                
                // substitute the pointe node.
               
                /*
                CFESkelAnimNode* m_poPointerNode = CFESkelAnimNodeInstancer::poCreateInstance( CFESkelAnimLoader::poLoadNode("data/actors/player/face") );
                CHUDMgr::SubstNode(m_poPointerNode,"LevelProgressGroup","ProgressPointer");
                CHUDMgr::SubstNode(m_poPointerNode,"LevelProgressGroup","ProgressPointer");
                
                m_poPointerNode->SetScale(CFEVect2(0.75,0.75));
                m_poPointerSpriteNode = (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode(m_poPointerNode,"face");
                CFESpriteInstMgr::Enable(m_poPointerSpriteNode->hGetSprite());
                m_poPointerSpriteNode->SetAction(0);
                */
            }
            break;
        }

        
    #else

        CFEString sHUDs[] = 
        {
            "data/HUD/HUD",
            ""
        };
        
        CHUDMgr::Init(sHUDs);

    #endif
}
// -----------------------------------------------------------------------------
void CHUDMgr::Init(CFEString _sHUDs[])
{
    /// 
    memset(m_oHUDActions,0,sizeof(m_oHUDActions));

	m_poPlayerEnergyAction      = NULL;
	m_poBossEnergyAction        = NULL;
	m_poPlayerProgressAction    = NULL;
	m_poLives[0] = NULL;
	m_poLives[1] = NULL;
	m_poLives[2] = NULL;
	
	// Retrieve HUD objects
    uint j = 0;
    while ((_sHUDs[j] != "") && (j<2))
    {
	    m_hHUD[j] = CFEHUDInstMgr::hGetInstance(_sHUDs[j]);
     
        /// Retrieve HUD Actions   
        uint i;
	    for (i=0;i<HA_NUM_HUD_ACTIONS;i++)
	    {
	        if (m_oHUDActions[i].m_poAction == NULL)
	        {
	            m_oHUDActions[i].m_bEnabled = false;
	            m_oHUDActions[i].m_rTime    = 0;
	            m_oHUDActions[i].m_poAction = CFEHUDInstMgr::poGetElementAction(m_hHUD[j],HUD_ACTIONS[i].m_sActionName);
	            m_oHUDActions[i].m_poObject = CFEHUDInstMgr::poGetObject(m_hHUD[j],HUD_ACTIONS[i].m_sGroupName);

	            if (m_oHUDActions[i].m_poObject!= NULL) 
		            m_oHUDActions[i].m_poObject->Hide();
            }
	    }
	    
	    /// Retrieve specific actions
    	if (m_poPlayerEnergyAction      == NULL) m_poPlayerEnergyAction   = CFEHUDInstMgr::poGetElementAction(m_hHUD[j],"EnergyAction");
        if (m_poBossEnergyAction        == NULL) m_poBossEnergyAction     = CFEHUDInstMgr::poGetElementAction(m_hHUD[j],"EnemyEnergyAction");
        if (m_poPlayerProgressAction    == NULL) m_poPlayerProgressAction = CFEHUDInstMgr::poGetElementAction(m_hHUD[j],"PlayerProgressAction");

        /// Retrieve specific objects
        for (i=0;i<MAX_LIVES;i++)
        {
            CFEString sLive = CFEString("Live") + CFEString(i);
            if (m_poLives[i] == NULL) m_poLives[i] = CFEHUDInstMgr::poGetObject(m_hHUD[j],sLive);
        }
        
        if (m_poLevelTitle == NULL) m_poLevelTitle = (CFEHUDLabel*)CFEHUDInstMgr::poGetObject(m_hHUD[j],"LevelNameValue");
        if (m_poScore == NULL) m_poScore = (CFEHUDLabel*)CFEHUDInstMgr::poGetObject(m_hHUD[j],"ScoreValue");

        j++;
    }
}
// -----------------------------------------------------------------------------
void CHUDMgr::Finish()
{
    memset(m_oHUDActions,0,sizeof(m_oHUDActions));
    m_hHUD[0] = m_hHUD[1] = NULL;

    m_poPlayerEnergyAction       = NULL;
    m_poBossEnergyAction         = NULL;
    m_poPlayerProgressAction     = NULL;
    
    m_poPointerNode = NULL;
    m_poPointerSpriteNode = NULL;

    memset(m_poLives,0,sizeof(m_poLives));
    m_poLevelTitle = NULL;
    m_poScore = NULL;
}
// -----------------------------------------------------------------------------
void CHUDMgr::Reset()
{
	// Reset items as at the start of the level
	CFEHUDUpdater::SetTime(1);
	CFEHUDUpdater::Process(m_poPlayerEnergyAction);

	CFEHUDUpdater::SetTime(1);
	CFEHUDUpdater::Process(m_poBossEnergyAction);

	CFEHUDUpdater::SetTime(0);
	CFEHUDUpdater::Process( m_oHUDActions[HA_SHOW_ENEMY_HUD].m_poAction );

	m_oHUDActions[HA_SHOW_ENEMY_HUD].m_rTime    = 0;
	m_oHUDActions[HA_SHOW_ENEMY_HUD].m_poObject->Hide();
	m_oHUDActions[HA_SHOW_ENEMY_HUD].m_bEnabled = false;
}
// -----------------------------------------------------------------------------
void CHUDMgr::SetPlayerEnergy(FEReal _rEnergy)
{
	if (m_poPlayerEnergyAction != NULL) 
	{
	    CFEHUDUpdater::SetTime(_rEnergy);
	    CFEHUDUpdater::Process(m_poPlayerEnergyAction);
    }
}
// -----------------------------------------------------------------------------
void CHUDMgr::SetPlayerLives(uint _uiLives)
{
	for (uint i=0;i<MAX_LIVES;i++)
	{
		if (m_poLives[i] != NULL)
		{
			if (i<_uiLives)
				m_poLives[i]->Show();
			else
				m_poLives[i]->Hide();
		}	
	}
}
// -----------------------------------------------------------------------------
void CHUDMgr::SetBossEnergy(FEReal _rEnergy)
{
	if (m_poBossEnergyAction != NULL) 
	{
	    CFEHUDUpdater::SetTime(_rEnergy);
	    CFEHUDUpdater::Process(m_poBossEnergyAction);
    }
}
// -----------------------------------------------------------------------------
void CHUDMgr::SetPlayerProgress(FEReal _rProgress)
{
    if (m_poPlayerProgressAction != NULL) 
	{
	    CFEHUDUpdater::SetTime(_rProgress);
	    CFEHUDUpdater::Process(m_poPlayerProgressAction);
    }
}
// -----------------------------------------------------------------------------
void CHUDMgr::SetProgressFace(uint _uiAction)
{
    if (m_poPointerSpriteNode != NULL) 
	{
	    m_poPointerSpriteNode->SetAction(_uiAction);
    }
}
// -----------------------------------------------------------------------------
void CHUDMgr::SetLevelTitle(const CFEString& _sLevelTitle)
{
    if (m_poLevelTitle != NULL) 
	{
	    m_poLevelTitle->SetText(_sLevelTitle);
    }
}
// -----------------------------------------------------------------------------
void CHUDMgr::SetScore(uint _uiScore)
{
    if (m_poScore != NULL)
	{
	    m_poScore->SetText( CFEString(_uiScore) );
    }
}
// -----------------------------------------------------------------------------
void CHUDMgr::Play(EHUDAction _eAction)
{
    if (_eAction != HA_HIDE_PLAYER_HUD)
    {
        m_oHUDActions[_eAction].m_bEnabled = true;
		m_oHUDActions[_eAction].m_rTime    = 0;
        m_oHUDActions[_eAction].m_poObject->Show();

        CFEHUDUpdater::SetTime(_0r);
        CFEHUDUpdater::Process(m_oHUDActions[_eAction].m_poAction);
	}
    else
    {
        m_oHUDActions[HA_SHOW_PLAYER_HUD].m_bEnabled = true;
		m_oHUDActions[HA_SHOW_PLAYER_HUD].m_rTime    = 0;
		m_oHUDActions[HA_SHOW_PLAYER_HUD].m_poObject->Hide();
    }
}
// -----------------------------------------------------------------------------
bool CHUDMgr::bPlayingAction(EHUDAction _eAction)
{
    return (m_oHUDActions[_eAction].m_bEnabled);
}
// -----------------------------------------------------------------------------
void CHUDMgr::Update(FEReal _rDeltaT)
{
	for (uint i=0;i<HA_NUM_HUD_ACTIONS;i++)
	{
		if (m_oHUDActions[i].m_bEnabled)
		{
			m_oHUDActions[i].m_rTime += _rDeltaT;
			if (m_oHUDActions[i].m_rTime < m_oHUDActions[i].m_poAction->rGetActionTime())
				CFEHUDInstMgr::SetActionTime(m_oHUDActions[i].m_poAction,m_oHUDActions[i].m_rTime);
			else
			{
				m_oHUDActions[i].m_bEnabled = false;

				if (
				    (i != HA_SHOW_PLAYER_HUD) 
				 && (i != HA_SHOW_ENEMY_HUD)
				 // && (i != CORTINILLAIN_ACTION)
				    )
                {
					m_oHUDActions[i].m_poObject->Hide();
                }
			}
		}
	}
}
// -----------------------------------------------------------------------------
void CHUDMgr::Render(CFERenderer* _poRenderer,uint _uiHUDIdx)
{
    if (m_hHUD[_uiHUDIdx] == NULL) return;

    uint uiVHeight = _poRenderer->uiGetScreenVHeight();
    uint uiVWidth  = _poRenderer->uiGetScreenVWidth();

    _poRenderer->SetScreenVWidth (640.0);
    _poRenderer->SetScreenVHeight(480.0);	
    _poRenderer->ResetCamera();

    // ÑAPA por como trabaja el editor de HUDs!!!
    _poRenderer->TranslateCamera((800-640)*0.5f,(600-480)*0.5f);
        
        CFEHUDInstMgr::Render(m_hHUD[_uiHUDIdx],_poRenderer);

    _poRenderer->SetScreenVWidth (uiVWidth );
    _poRenderer->SetScreenVHeight(uiVHeight);
}
// -----------------------------------------------------------------------------
