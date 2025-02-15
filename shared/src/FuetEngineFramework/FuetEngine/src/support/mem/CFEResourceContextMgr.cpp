// ----------------------------------------------------------------------------
/*! \class CFEMem
 *  \brief Basic Memory Management Functionalities
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include "CFEResourceContextMgr.h"

#include "CFEMem.h"
#include "Support/Sound/CFESoundMgr.h"
#include "Support/Graphics/CFEMaterialMgr.h"
#include "Graphics/Font/CFEFontMgr.h"
#include "Graphics/Sprite/CFESpriteMgr.h"
#include "Graphics/Sprite/CFESpriteInstMgr.h"
#include "Graphics/skelanim/CFESkelAnimMgr.h"
#include "Graphics/skelanim/CFESkelAnimInstMgr.h"
#include "Graphics/particlesys/CFEParticleMgr.h"
#include "Graphics/particlesys/CFEParticleSysInstMgr.h"
#include "Graphics/particlesys/CFEParticleSysMgr.h"
// ----------------------------------------------------------------------------
static uint gsuiContextID = 1;
// ----------------------------------------------------------------------------
void CFEResourceContextMgr::Init(uint _uiMaxMemContexts)
{
	/// CFEMem::Init(_uiMaxMemContexts);
	gsuiContextID = 1;
}
// ----------------------------------------------------------------------------
void CFEResourceContextMgr::Finish()
{
	while (gsuiContextID>0)
		PopContext();

	/// CFEMem::Finish();
	gsuiContextID = 1;
}
// ----------------------------------------------------------------------------
void CFEResourceContextMgr::PushContext()
{
	gsuiContextID++;
	// CFEMem::PushContext();
}
// ----------------------------------------------------------------------------
void CFEResourceContextMgr::PopContext()
{
	// It should be good if the resource managers can be registerables but,
	// at this moment it's not possible to do so. We do this using the already known managers.
	CFESoundMgr::ReleaseContextResources(gsuiContextID);

	CFESkelAnimInstMgr::ReleaseContextInstances(gsuiContextID);
	CFESkelAnimMgr::ReleaseContextResources(gsuiContextID);

	CFEParticleSysInstMgr::ReleaseContextInstances(gsuiContextID);
	CFEParticleSysMgr::ReleaseContextResources(gsuiContextID);
	// CFEParticleMgr::ReleaseContextResources(gsuiContextID);

	CFESpriteInstMgr::ReleaseContextInstances(gsuiContextID);
	CFESpriteMgr::ReleaseContextResources(gsuiContextID);

	CFEFontMgr::ReleaseContextResources(gsuiContextID);
	CFEMaterialMgr::ReleaseContextResources(gsuiContextID);

	/*
	if (gsuiContextID>1)
		CFEMem::PopContext();
	else
		CFEMem::ResetContext();
	*/

	gsuiContextID--;
}
// ----------------------------------------------------------------------------
uint CFEResourceContextMgr::uiGetContextID()
{
	return(gsuiContextID);
}
// ----------------------------------------------------------------------------
