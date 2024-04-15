// ----------------------------------------------------------------------------
/*! \class CFEMeshLoader
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEEnums.h"
#include "CFEMeshLoader.h"
// #include "System/FMem.h"
#include "support/file/CFEFile.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "support/misc/CFEStringUtils.h"
#include "Core/CFECore.h"

// ----------------------------------------------------------------------------
CFEMesh* CFEMeshLoader::poBuildBasicMesh(FEHandler _hMat,const CFEString& _sMeshName)
{
    CFEMesh* poMesh = new CFEMesh;
    poMesh->SetName(_sMeshName);

    /*
    CFEMeshAction oAction;
    oAction.m_ePlayMode = MAPM_ONESHOT;
	oAction.m_rActionTime = _0r;
	oAction.m_rRandStartTime = _0r;

    CFEMeshFrame* poFrame;
    oFrame.m_rBlendTime = _0r;
    oFrame.m_rDelay     = _0r;
    oFrame.m_rStartTime = _0r;
    oFrame.m_rFrameTime = _0r;

    oAction.m_oSeq.push_back(oFrame);
    poMesh->m_oActions.push_back(oAction);
	*/

    return( poMesh );
}
// ----------------------------------------------------------------------------
CFEMesh* CFEMeshLoader::poLoad(const CFEString& _sFilename)
{
    CFEString sFilename = _sFilename + ".msh";
    CFEString sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEFile oFile;
	if (! oFile.bOpen(sFilename,FOM_READ)) return(NULL);

	// --------------------------
	// read File ID
	char szID[5] = "";
	oFile.uiRead((FEPointer)szID,4);
	if (CFEString(szID) != "MESH")
	{
		// bad id
		oFile.Close();
		return(NULL);
	}

	CFEMesh* poMesh = new CFEMesh;

	// mesh name
	char szMeshName[16];
	oFile.uiRead((FEPointer)szMeshName,16);
	poMesh->SetName(szMeshName);

	// material name
	char szMaterial[32];
	oFile.uiRead((FEPointer)szMaterial,32);
	poMesh->m_hSpriteInst = CFESpriteInstMgr::I()->hGetInstance(sWorkingDir + CFEString("/") + szMaterial);

	// --------------------------
	// Setup preferred memory alignment
	uint uiOldAlignment;
	uint uiMemAlignment = 32;
	CFECore::Mem::bGetProperty("Alignment",(FEPointer)&uiOldAlignment);
	CFECore::Mem::bSetProperty("Alignment",(FEPointer)&uiMemAlignment);

	// --------------------------
	// number of indices:
	uint uiNumIdx;
	oFile.uiReadArray32((FEPointer)&uiNumIdx,1);

	if (uiNumIdx>0)
	{
		for(uint i=0;i<uiNumIdx;i++)
			poMesh->m_usIdx.push_back(i);
		// poMesh->m_uiIdx.reserve(uiNumIdx);
	}

	// index array
	oFile.uiReadArray16((FEPointer)&poMesh->m_usIdx[0],uiNumIdx);

	// --------------------------
	// number of vertices:
	uint uiNumVXs;
	oFile.uiReadArray32((FEPointer)&uiNumVXs,1);

	if (uiNumVXs>0)
	{
		for(uint i=0;i<uiNumVXs;i++)
			poMesh->m_oUV.push_back(CFEVect2::ZERO());
		// poMesh->m_oUV.reserve(uiNumVXs);
	}

	// UV array
	oFile.uiReadArray32((FEPointer)&poMesh->m_oUV[0],uiNumVXs*2);

	// --------------------------
	// Read actions
	uint uiNumActions;
	oFile.uiReadArray32((FEPointer)&uiNumActions,1);

	for (uint a=0;a<uiNumActions;a++)
	{
		CFEMeshAction* poAction = new CFEMeshAction;

		// action name
		char szActionName[32];
		oFile.uiRead((FEPointer)szActionName,32);
		poAction->SetName(szActionName);

		//
		oFile.uiReadArray32((FEPointer)&poAction->m_rActionTime,1);
		oFile.uiReadArray32((FEPointer)&poAction->m_rRandStartTime,1);
		oFile.uiReadArray32((FEPointer)&poAction->m_ePlayMode,1);

		// Read action frames
		uint uiNumFrames;
		oFile.uiReadArray32((FEPointer)&uiNumFrames,1);

		for (uint f=0;f<uiNumFrames;f++)
		{
			CFEMeshFrame* poActionFrame = new CFEMeshFrame;

			//
			oFile.uiReadArray32((FEPointer)&poActionFrame->m_rBlendTime,1);
			oFile.uiReadArray32((FEPointer)&poActionFrame->m_rDelay,1);
			oFile.uiReadArray32((FEPointer)&poActionFrame->m_rFrameTime,1);
			oFile.uiReadArray32((FEPointer)&poActionFrame->m_rStartTime,1);

			// read frame VXs
			if (uiNumVXs>0)
			{
				for(uint i=0;i<uiNumVXs;i++)
					poActionFrame->m_oVX.push_back(CFEVect2::ZERO());
				// poActionFrame->m_oVX.reserve(uiNumVXs);
			}

			// VX array
			oFile.uiReadArray32((FEPointer)&poActionFrame->m_oVX[0],uiNumVXs*2);

			poAction->m_oSeq.push_back(poActionFrame);
		}

		poMesh->m_poActions.push_back(poAction);
		oFile.Close();
	}
	
	// Restore old alignment
	CFECore::Mem::bSetProperty("Alignment",(FEPointer)&uiOldAlignment);

    return( poMesh );
}
// ----------------------------------------------------------------------------
