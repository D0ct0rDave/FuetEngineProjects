// ----------------------------------------------------------------------------
/*! \class CFESkelAnimNodeInstancerINT
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "support/graphics/CFEMaterialMgr.h"

#include "CFESkelAnimNodeInstancer.h"
#include "CFESkelAnim.h"
#include "CFESkelAnimGroup.h"

#include "CFESkelAnimSpriteModel.h"
#include "CFESkelAnimSprite.h"

#include "CFESkelAnimMeshModel.h"
#include "CFESkelAnimMesh.h"

#include "CFESkelAnimBone.h"

#include "CFESkelAnimNodeTableBuilder.h"
// ----------------------------------------------------------------------------
class CFESkelAnimNodeInstancerINT : public CFESkelAnimNodeVisitor
{
	public:

		CFESkelAnimNode* poCreateInstance(CFESkelAnimNode* _poNode);
		
		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj);
        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj); 
        /// 
        virtual void Visit(CFESkelAnimSpriteModel* _poObj);
        /// 
        virtual void Visit(CFESkelAnimBone* _poObj); 
        /// 
        virtual void Visit(CFESkelAnimMesh* _poObj);
	    /// 
        virtual void Visit(CFESkelAnimMeshModel* _poObj);
	
	    /// Find an already instanced bone in the hierarchy.
        CFESkelAnimBone* poFindBone(const CFEString& _sName)
        {
            for (uint i=0;i<m_poInstancedBones.size();i++)
                if (m_poInstancedBones[i]->sGetName() == _sName)
                    return(m_poInstancedBones[i]);

            return(NULL);
        }
        
        void CopyCommonProperties(CFESkelAnimNode* _poSrcNode,CFESkelAnimNode* _poDstNode);

	protected:

		CFESkelAnimNode* m_poInstance;
		CFEArray<CFESkelAnimBone*> m_poInstancedBones;
};
// ----------------------------------------------------------------------------
CFESkelAnimNode* CFESkelAnimNodeInstancerINT::poCreateInstance(CFESkelAnimNode* _poNode)
{
	m_poInstance = NULL;

	_poNode->Accept(this);
	m_poInstancedBones.clear();
	
	return(m_poInstance);
}
// ----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::CopyCommonProperties(CFESkelAnimNode* _poSrcNode,CFESkelAnimNode* _poDstNode)
{
	// Copy common properties
	_poDstNode->SetPos	( _poSrcNode->oGetPos() );
	_poDstNode->SetAngle( _poSrcNode->rGetAngle() );

	#ifdef USE_SKELANIM_SCALE_COLOR
	_poDstNode->SetScale( _poSrcNode->oGetScale() );
	_poDstNode->SetColor( _poSrcNode->oGetColor() );
	#endif

	_poDstNode->Show	( _poSrcNode->bIsVisible() );
	_poDstNode->SetDepth( _poSrcNode->rGetDepth() );
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimGroup* _poObj)
{    
	CFESkelAnimGroup* poGroup = new CFESkelAnimGroup(_poObj->sGetName());
	CopyCommonProperties(_poObj,poGroup);

	// Copy specific properties
	for (uint i=0;i<_poObj->uiNumChildren();i++)
	{
		_poObj->poGetChild(i)->Accept(this);
		poGroup->uiAddChild(m_poInstance);
	}

	m_poInstance = poGroup;
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimSprite* _poObj)
{
	CFESkelAnimSprite* poSpriteAnim = new CFESkelAnimSprite(_poObj->sGetName());
	CopyCommonProperties(_poObj,poSpriteAnim);

	// Get the sprite model from the original sprite instance
	FEHandler hInst = CFESpriteInstMgr::I()->hGetInstance( CFESpriteInstMgr::I()->poGetSprite(_poObj->hGetSprite()) );
	if (hInst != NULL)
    {
		CFESpriteInstMgr::I()->ManageRender(hInst,false);
		poSpriteAnim->SetSprite( hInst );
		poSpriteAnim->SetAction( _poObj->uiGetAction() );
	}

	m_poInstance = poSpriteAnim;
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimSpriteModel* _poObj)
{
	// Create an sprite instance from a sprite model
	CFESkelAnimSprite* poSpriteAnim = new CFESkelAnimSprite(_poObj->sGetName());
	CopyCommonProperties(_poObj,poSpriteAnim);

    FEHandler hInst = CFESpriteInstMgr::I()->hGetInstance( _poObj->sGetSprite() );
    if (hInst != NULL)
    {
        CFESpriteInstMgr::I()->ManageRender(hInst,false);
        poSpriteAnim->SetSprite( hInst );
        poSpriteAnim->SetAction( _poObj->uiGetSpriteAction() );
    }

	m_poInstance = poSpriteAnim;
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimBone* _poObj)
{
	CFESkelAnimBone* poBone = poFindBone(_poObj->sGetName());
	if (poBone == NULL)
	{
	    poBone = new CFESkelAnimBone(_poObj->sGetName());
        m_poInstancedBones.push_back(poBone);
    }
	CopyCommonProperties(_poObj,poBone);


	poBone->SetBonePos(_poObj->oGetBonePos());
	for (uint i=0;i<_poObj->uiNumChildren();i++)
	{
		_poObj->poGetChild(i)->Accept(this);
		poBone->uiAddChild(m_poInstance);
	}

	m_poInstance = poBone;
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimMesh* _poObj)
{
	CFESkelAnimMesh* poMesh = new CFESkelAnimMesh(_poObj->sGetName());
	CopyCommonProperties(_poObj,poMesh);

    for (uint i=0;i<_poObj->uiGetNumAttachedBones();i++)
    {
        CFESkelAnimBone* poOriginalBone = _poObj->poGetAttachedBone(i);
		if (poOriginalBone != NULL)
		{
			CFEString sBoneName = poOriginalBone->sGetName();
			CFESkelAnimBone* poBone = poFindBone(sBoneName);

			if (poBone == NULL)
			{
				// Create a basic bone object. Will (hopefully) setup later
				poBone = new CFESkelAnimBone(sBoneName);
				m_poInstancedBones.push_back(poBone);
			}

	        poMesh->AttachBone(poBone);
		}
		else
			poMesh->AttachBone(NULL);
    }

	// Get the sprite model from the original sprite instance
	FEHandler hInst = CFESpriteInstMgr::I()->hGetInstance( CFESpriteInstMgr::I()->poGetSprite( _poObj->hGetSprite() ));
	if (hInst != NULL)
    {
		CFESpriteInstMgr::I()->ManageRender(hInst,false);
		poMesh->SetSprite( hInst );
		poMesh->SetAction( _poObj->uiGetAction() );
	}

    // qué pasa con todos los bones attachados a la mesh a instanciar ????
	m_poInstance = poMesh;
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimMeshModel* _poObj)
{
	CFESkelAnimMesh* poMesh = new CFESkelAnimMesh(_poObj->sGetName());
	CopyCommonProperties(_poObj,poMesh);
	
    for (uint i=0;i<_poObj->uiGetNumAttachedBones();i++)
    {
        CFEString sBoneName = _poObj->sGetAttachedBone(i);
	    CFESkelAnimBone* poBone = poFindBone( sBoneName );

	    if (poBone == NULL)
	    {
	        // Create a basic bone object. Will (hopefully) setup later
	        poBone = new CFESkelAnimBone(sBoneName);
            m_poInstancedBones.push_back(poBone);
        }
        
        poMesh->AttachBone(poBone);
    }

    FEHandler hInst = CFESpriteInstMgr::I()->hGetInstance( _poObj->sGetSprite() );
    if (hInst != NULL)
    {
        CFESpriteInstMgr::I()->ManageRender(hInst,false);
        poMesh->SetSprite( hInst );
        poMesh->SetAction( _poObj->uiGetSpriteAction() );
    }

    // qué pasa con todos los bones attachados a la mesh a instanciar ????
	m_poInstance = poMesh;
}
//-----------------------------------------------------------------------------
CFESkelAnimNode* CFESkelAnimNodeInstancer::poCreateInstance(CFESkelAnimNode* _poSkelAnimNode)
{
	if (_poSkelAnimNode == NULL) return(NULL);

	// Retrieve an instance of the anim node
	CFESkelAnimNodeInstancerINT oInstancer;
	return( oInstancer.poCreateInstance( _poSkelAnimNode ) );
}
//-----------------------------------------------------------------------------
