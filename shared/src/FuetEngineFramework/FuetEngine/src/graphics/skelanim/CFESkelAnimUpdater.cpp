// ----------------------------------------------------------------------------
/*! \class CFESkelAnimUpdater
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimUpdater.h"

#include "CFESkelAnim.h"
#include "types\FEKFBFLerpFuncs.h"
//-----------------------------------------------------------------------------
void CFESkelAnimUpdater::Update(CFESkelAnimInst* _poInstance)
{
	CFESkelAnim* poAnim	= _poInstance->m_poSkelAnim;
	if (poAnim == NULL) return;

	CFESkelAnimActionSet* poActionSet = poAnim->poGetAnimActionSet();
	if (poActionSet == NULL) return;
	
	CFESkelAnimAction* poAction = poAnim->poGetAnimActionSet()->poGetAction(_poInstance->m_uiAction);
    if (poAction == NULL) return;

	for (uint i=0;i<poAction->uiNumNodeActions();i++)
	{
		CFESkelAnimNodeAction* poNodeAction = poAction->poGetNodeAction(i);

		// Retrieve the node index this node action should affect.
		uint uiIdx = poNodeAction->uiGetNodeIdx();

		// Retrieve the node related to the index.
		CFESkelAnimNode* poNode		= poAnim->m_oNodeTab[uiIdx];
		CFESkelAnimNode* poInstNode = (*_poInstance->m_poNodeInstTab)[uiIdx];

		// Update the node.
		poNodeAction->SetupNode(_poInstance->m_rActionTime,poInstNode);

		// Apply original node properties.
		poInstNode->SetScale( poInstNode->oGetScale() * poNode->oGetScale() );
		poInstNode->SetPos( poInstNode->oGetPos() + poNode->oGetPos() );
		poInstNode->SetAngle( poInstNode->rGetAngle() + poNode->rGetAngle() );
		poInstNode->SetColor( poInstNode->oGetColor() * poNode->oGetColor() );
	}
}
//-----------------------------------------------------------------------------
