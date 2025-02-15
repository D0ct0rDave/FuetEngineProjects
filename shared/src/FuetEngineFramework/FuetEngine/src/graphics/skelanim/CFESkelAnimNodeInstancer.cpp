// ----------------------------------------------------------------------------
/*! \class CFESkelAnimNodeInstancerINT
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimGroup* _poObj)
{    
	CFESkelAnimGroup* poGroup = new CFESkelAnimGroup(_poObj->sGetName());

	poGroup->SetPos( _poObj->oGetPos() );
	poGroup->SetScale ( _poObj->oGetScale() );
	poGroup->SetAngle( _poObj->rGetAngle() );
	poGroup->SetColor( _poObj->oGetColor() );
	poGroup->Show( _poObj->bIsVisible() );
	poGroup->SetDepth( _poObj->rGetDepth() );

	for (uint i=0;i<_poObj->uiNumObjs();i++)
	{
		_poObj->poGetNode(i)->Accept(this);
		poGroup->uiAddNode(m_poInstance);
	}

	m_poInstance = poGroup;
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimSprite* _poObj)
{
	CFESkelAnimSprite* poSpriteAnim = new CFESkelAnimSprite(_poObj->sGetName());
	
	// Copy properties
	poSpriteAnim->SetPos( _poObj->oGetPos() );
	poSpriteAnim->SetScale ( _poObj->oGetScale() );
	poSpriteAnim->SetAngle( _poObj->rGetAngle() );
	poSpriteAnim->SetColor( _poObj->oGetColor() );
	poSpriteAnim->Show( _poObj->bIsVisible() );
	poSpriteAnim->SetDepth( _poObj->rGetDepth() );

	// Get the sprite model from the original sprite instance
	FEHandler hInst = CFESpriteInstMgr::hGetInstance( CFESpriteInstMgr::poGetSprite(_poObj->hGetSprite()) );
	if (hInst != NULL)
    {
		CFESpriteInstMgr::ManageRender(hInst,false);
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

	// Copy properties
	poSpriteAnim->SetPos( _poObj->oGetPos() );
	poSpriteAnim->SetScale ( _poObj->oGetScale() );
	poSpriteAnim->SetAngle( _poObj->rGetAngle() );
	poSpriteAnim->SetColor( _poObj->oGetColor() );
	poSpriteAnim->Show( _poObj->bIsVisible() );
	poSpriteAnim->SetDepth( _poObj->rGetDepth() );

    FEHandler hInst = CFESpriteInstMgr::hGetInstance( _poObj->sGetSprite() );
    if (hInst != NULL)
    {
        CFESpriteInstMgr::ManageRender(hInst,false);
        poSpriteAnim->SetSprite( hInst );
        poSpriteAnim->SetAction( 0 );
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

	poBone->SetPos( _poObj->oGetPos() );
	poBone->SetScale ( _poObj->oGetScale() );
	poBone->SetAngle( _poObj->rGetAngle() );
	poBone->SetColor( _poObj->oGetColor() );
	poBone->Show( _poObj->bIsVisible() );
	poBone->SetDepth( _poObj->rGetDepth() );

    if (_poObj->poGetAttachedNode())
    {
        _poObj->poGetAttachedNode()->Accept(this);
        poBone->SetAttachedNode( m_poInstance );
    }

	m_poInstance = poBone;
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeInstancerINT::Visit(CFESkelAnimMesh* _poObj)
{
	CFESkelAnimMesh* poMesh = new CFESkelAnimMesh(_poObj->sGetName());

	poMesh->SetPos( _poObj->oGetPos() );
	poMesh->SetScale ( _poObj->oGetScale() );
	poMesh->SetAngle( _poObj->rGetAngle() );
	poMesh->SetColor( _poObj->oGetColor() );
	poMesh->Show( _poObj->bIsVisible() );
	poMesh->SetDepth( _poObj->rGetDepth() );

    for (uint i=0;i<_poObj->uiGetNumAttachedBones();i++)
    {
        CFESkelAnimBone* poOriginalBone = _poObj->poGetAttachedBone(i);
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

	// Get the sprite model from the original sprite instance
	FEHandler hInst = CFESpriteInstMgr::hGetInstance( CFESpriteInstMgr::poGetSprite( _poObj->hGetSprite() ));
	if (hInst != NULL)
    {
		CFESpriteInstMgr::ManageRender(hInst,false);
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

	poMesh->SetPos( _poObj->oGetPos() );
	poMesh->SetScale ( _poObj->oGetScale() );
	poMesh->SetAngle( _poObj->rGetAngle() );
	poMesh->SetColor( _poObj->oGetColor() );
	poMesh->Show( _poObj->bIsVisible() );
	poMesh->SetDepth( _poObj->rGetDepth() );

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

    FEHandler hInst = CFESpriteInstMgr::hGetInstance( _poObj->sGetSprite() );
    if (hInst != NULL)
    {
        CFESpriteInstMgr::ManageRender(hInst,false);
        poMesh->SetSprite( hInst );
        poMesh->SetAction( 0 );
    }

	/*
	/// Sets the material for this mesh.
    poMesh->SetMaterial( CFEMaterialMgr::hLoad(_poObj->sGetMaterial()) );
    */
		

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
