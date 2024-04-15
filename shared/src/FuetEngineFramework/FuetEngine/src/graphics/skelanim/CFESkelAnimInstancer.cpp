// ----------------------------------------------------------------------------
/*! \class CFESkelAnimInstancer
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

#include "CFESkelAnimInstancer.h"
#include "CFESkelAnim.h"
#include "CFESkelAnimGroup.h"

#include "CFESkelAnimSpriteModel.h"
#include "CFESkelAnimSprite.h"

#include "CFESkelAnimMeshModel.h"
#include "CFESkelAnimMesh.h"

#include "CFESkelAnimBone.h"

#include "CFESkelAnimNodeTableBuilder.h"
#include "CFESkelAnimNodeInstancer.h"

//-----------------------------------------------------------------------------
void CFESkelAnimInstancer::CreateInstance(CFESkelAnim* _poSkelAnim,CFESkelAnimInst* _poSkelAnimInstance)
{
	if ((_poSkelAnimInstance == NULL) || (_poSkelAnim==NULL)) return;

	// Retrieve an instance of the anim node
	_poSkelAnimInstance->m_poInstAnimNode = CFESkelAnimNodeInstancer::poCreateInstance( _poSkelAnim->poGetAnimNode() );
	_poSkelAnimInstance->m_poNodeInstTab  = new CFEArray<CFESkelAnimNode*>;

	CFESkelAnimNodeTableBuilder::BuildTable(_poSkelAnimInstance->m_poNodeInstTab,_poSkelAnimInstance->m_poInstAnimNode);

	_poSkelAnimInstance->m_poNodeInstMask = new FEBool[ _poSkelAnimInstance->m_poNodeInstTab->size() ];

	for (uint i=0;i<_poSkelAnimInstance->m_poNodeInstTab->size();i++)
		_poSkelAnimInstance->m_poNodeInstMask[i] = false;
}
//-----------------------------------------------------------------------------
