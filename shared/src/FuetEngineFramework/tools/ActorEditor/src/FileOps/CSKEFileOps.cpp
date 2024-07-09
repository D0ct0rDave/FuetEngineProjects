 //---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CFileOpsCommon.h"
#include "CSKEFileOps.h"

#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
#include "edutils.h"
//---------------------------------------------------------------------------
CSkeletonNodeWritter::CSkeletonNodeWritter() : m_poFW(NULL) {};

void CSkeletonNodeWritter::Write(CFESkelAnimNode* _poNode,CConfigFileWriter* _poFW,const CFEString& _sWorkingDir,bool _bDescentBones)
{
	CSkeletonNodeWritter oObj;
	oObj.m_poFW			= _poFW;
	oObj.m_sWorkingDir	= _sWorkingDir;
	oObj.m_bDescentBones = _bDescentBones;

	_poNode->Accept(&oObj);
}

void CSkeletonNodeWritter::WriteCommonProperties(CFESkelAnimNode* _poNode)
{
	m_poFW->AddVar("Name", _poNode->sGetName().szString(),"");

	m_poFW->OpenSection("Position");
		m_poFW->AddVar("x", _poNode->oGetPos().x,DEF_POS_X);
		m_poFW->AddVar("y", _poNode->oGetPos().y,DEF_POS_Y);
	m_poFW->CloseSection(); // pos

	
	// These properties are discardable since they are not edited by the application
	m_poFW->AddVar("Visible", (bool)_poNode->bIsVisible(),true);

	m_poFW->AddVar("Depth", _poNode->rGetDepth(),DEF_DEPTH, 6);
	
	m_poFW->AddVar("Angle", _poNode->rGetAngle(),DEF_ANGLE);

	#ifdef USE_SKELANIM_SCALE_COLOR
	m_poFW->OpenSection("Scale");
		m_poFW->AddVar("x", _poNode->oGetScale().x,DEF_SCALE_X);
		m_poFW->AddVar("y", _poNode->oGetScale().y,DEF_SCALE_Y);
	m_poFW->CloseSection(); // scale

	m_poFW->OpenSection("Color");
		m_poFW->AddVar("r", _poNode->oGetColor().r,1.0f);
		m_poFW->AddVar("g", _poNode->oGetColor().g,1.0f);
		m_poFW->AddVar("b", _poNode->oGetColor().b,1.0f);
		m_poFW->AddVar("a", _poNode->oGetColor().a,1.0f);
	m_poFW->CloseSection(); // color
	#endif
}

void CSkeletonNodeWritter::Visit(CFESkelAnimGroup* _poObj)
{
	m_poFW->AddVar("Type","group","");
	WriteCommonProperties(_poObj);

	uint uiWrittenNodes = 0;
	for (uint i=0;i<_poObj->uiNumChildren();i++)
	{
		CFEString sVar = CFEString("Child") + CFEString((int)i);

		CFESkelAnimNode* poNode = _poObj->poGetChild(i);
		if (poNode != NULL)
		{
			if ((m_bDescentBones) || (CFESkelAnimNodeTypeGetter::eGetType(poNode) != SANT_BONE))
			{
				m_poFW->OpenSection(sVar.szString());

					poNode->Accept(this);

				m_poFW->CloseSection();
				uiWrittenNodes++; 
			}
		}
	}

	m_poFW->AddVar("NumChildren",uiWrittenNodes,0);
}

/// 
void CSkeletonNodeWritter::Visit(CFESkelAnimSprite* _poObj)
{
	m_poFW->AddVar("Type","sprite","");
	WriteNodeWithSprite((CFESkelAnimSprite*)_poObj);
}

/// 
void CSkeletonNodeWritter::Visit(CFESkelAnimMesh* _poObj)
{
	m_poFW->AddVar("Type","mesh","");
	WriteNodeWithSprite((CFESkelAnimMesh*)_poObj);

	m_poFW->AddVar("NumAttachedBones",0,_poObj->uiGetNumAttachedBones());

	for (uint i=0;i<_poObj->uiGetNumAttachedBones();i++)
	{
		CFEString sVar = CFEString("AttachedBone") + CFEString((int)i);
		if (_poObj->poGetAttachedBone(i) != NULL)
			m_poFW->AddVar( sVar.szString(),_poObj->poGetAttachedBone(i)->sGetName().szString(),"");
		else
			m_poFW->AddVar( sVar.szString(),"","XXXX");
	}
}

/// 
void CSkeletonNodeWritter::Visit(CFESkelAnimBone* _poObj)
{
	if (! m_bDescentBones) return;

	m_poFW->AddVar("Type","bone","");
	WriteCommonProperties(_poObj);
	
	m_poFW->OpenSection("BonePos");

		m_poFW->AddVar("x",_poObj->oGetBonePos().x,DEF_POS_X);
		m_poFW->AddVar("y",_poObj->oGetBonePos().y,DEF_POS_Y);

	m_poFW->CloseSection();

	uint uiWrittenNodes = 0;
	for (uint i=0;i<_poObj->uiNumChildren();i++)
	{
		CFEString sVar = CFEString("Child") + CFEString((int)i);

		CFESkelAnimNode* poNode = _poObj->poGetChild(i);
		if (poNode != NULL)
		{
			m_poFW->OpenSection(sVar.szString());

				poNode->Accept(this);

			m_poFW->CloseSection();
			uiWrittenNodes++; 
		}
	}
	m_poFW->AddVar("NumChildren",uiWrittenNodes,0);	
}
//---------------------------------------------------------------------------
void SaveSkeleton(const CFEString& _sFilename,CSkeleton* _poSkeleton)
{
	// make backup of file
	if (bFileExists(_sFilename))	
		MakeBackupOfFile( wxString(_sFilename.szString()) );

    CFEString sWorkingDir = CFEStringUtils::sGetPath( _sFilename );
    CLibConfigFileWriter oFW(std::string( _sFilename.szString()) );

	CFEVect2 oOldPos = _poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBone->oGetBonePos();
	_poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBone->SetBonePos(CFEVect2::ZERO());

		oFW.OpenSection("SkelNode");

			CSkeletonNodeWritter::Write(_poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBone,&oFW,sWorkingDir);

		oFW.CloseSection(); // SkelNode
		oFW.Write();

    _poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBone->SetBonePos(oOldPos);
}
//---------------------------------------------------------------------------
void LoadSkeleton(const CFEString& _sFilename,CSkeleton* _poSkeleton)
{
    // TODO: Reimplement Load Skeleton function to work properly ...
    CFEString sFilename = CFEStringUtils::sRemoveExtension( CFEStringUtils::sNormalizePath(_sFilename) );
    CFESkelAnimNode* poLoadedSkeleton = CFESkelAnimLoader::poLoadNode(sFilename);

    if (poLoadedSkeleton!= NULL)
    {
		// we want to leave pivot bone position as is, so proceed with its children...
		for (uint i=0;i<_poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBone->uiNumChildren();i++)
		{
			CFESkelAnimNode* poNodeGraph = _poSkeleton->m_oBone[ _poSkeleton->m_uiPivotIdx ].m_poBone->poGetChild(i);
			CloneNodes(poNodeGraph,poLoadedSkeleton);
			
			// Clone also the instance
			poNodeGraph = CFESkelAnimNodeLocator::poLocateNode( 
									CFESkelAnimInstMgr::I()->poGetInstancedNode(globals.m_poSkeleton->m_hInst), 
									poNodeGraph->sGetName() );

			CloneNodes(poNodeGraph,poLoadedSkeleton);
		}
	}
}
//---------------------------------------------------------------------------
