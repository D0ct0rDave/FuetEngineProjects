// ----------------------------------------------------------------------------
/*! \class CFESkelAnimUpdater
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimUpdater.h"

#include "CFESkelAnim.h"
#include "CFESkelAnimSprite.h"
#include "CFESkelAnimMesh.h"
#include "CFESkelAnimBone.h"
#include "CFESkelAnimGroup.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "types\FEKFBFLerpFuncs.h"

//-----------------------------------------------------------------------------
class CFESkelAnimNodeSynchronizer: public CFESkelAnimNodeVisitor
{
	public:
		static void Sync(CFESkelAnimNode* _poRoot,FEReal _rTime)
		{
			CFESkelAnimNodeSynchronizer oObj(_rTime);
			_poRoot->Accept(&oObj);
		}

		CFESkelAnimNodeSynchronizer(FEReal _rTime)
		{
			m_rTime		= _rTime;
		}


		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			CFESpriteInstMgr::I()->SetCurrentActionTime( _poObj->hGetSprite(), m_rTime);				
        }

        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			CFESpriteInstMgr::I()->SetCurrentActionTime( _poObj->hGetSprite(), m_rTime);
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
            for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

	protected:

		FEReal	m_rTime;
};
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

		if (_poInstance->m_poNodeInstMask[uiIdx] == false)
		{
			// Retrieve the node related to the index.
			CFESkelAnimNode* poNode		= poAnim->m_oNodeTab[uiIdx];

			// Retrieve the instanced node related to the index.
			CFESkelAnimNode* poInstNode = (*_poInstance->m_poNodeInstTab)[uiIdx];

			// Update the node.
			poNodeAction->SetupNode(_poInstance->m_rActionTime,poInstNode);

			if (poInstNode->bIsVisible())
			{
				//poInstNode->SetPos  ( poNodeAction->m_oPosFunc.oGetValue(_poInstance->m_rActionTime) );
				//poInstNode->SetScale( poNodeAction->m_oScaleFunc.oGetValue(_poInstance->m_rActionTime) );
				//poInstNode->SetAngle( poNodeAction->m_rAngleFunc.oGetValue(_poInstance->m_rActionTime) );
				//poInstNode->SetColor( poNodeAction->m_oColorFunc.oGetValue(_poInstance->m_rActionTime) );

				// Apply original node properties (from the skeleton?)
				poInstNode->SetPos  ( poInstNode->oGetPos()   + poNode->oGetPos()   );
				poInstNode->SetAngle( poInstNode->rGetAngle() + poNode->rGetAngle() );
				
				#ifdef USE_SKELANIM_SCALE_COLOR
				poInstNode->SetScale( poInstNode->oGetScale() * poNode->oGetScale() );
				poInstNode->SetColor( poInstNode->oGetColor() * poNode->oGetColor() );
				#endif

				/*
				if (poNodeAction->iGetNodeActionIdx() > -1)
				{
					// to syncronize sprite/mesh animations with skeletal animatinos
					CFESkelAnimNodeSynchronizer::Sync(poInstNode,_poInstance->m_rActionTime);
				}*/

			}
		}
	}
}
//-----------------------------------------------------------------------------
// El sistema de eventos no funciona correctamente si varias instancias hacen uso de
// las mismas animaciones que lanzan eventos. Dentro de los nodos instanciados deberíamos
// guardar unas "cookies" que contuvieran información necesaria para procesar los eventos
// como LastEventT etc ... de este modo, sí que funcionaría.
//-----------------------------------------------------------------------------
void CFESkelAnimUpdater::RestartActions(CFESkelAnimInst* _poInstance)
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

		// Retrieve the instanced node related to the index.
		CFESkelAnimNode* poInstNode = (*_poInstance->m_poNodeInstTab)[uiIdx];

		if (poNodeAction != NULL)
			poNodeAction->m_oEventFunc.Reset();
	}
}
//-----------------------------------------------------------------------------
