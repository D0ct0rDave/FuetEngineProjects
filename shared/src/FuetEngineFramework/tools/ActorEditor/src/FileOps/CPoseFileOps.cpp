
//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CFileOpsCommon.h"
#include "CPoseFileOps.h"

#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
#include "edutils.h"

//---------------------------------------------------------------------------
void SavePoseFile(const CFEString& _sFilename,CSkeleton* _poSkeleton,float _fAnimTime)
{
	// make backup of file
	if (bFileExists(_sFilename))	
		MakeBackupOfFile( wxString(_sFilename.szString()) );

    CLibConfigFileWriter oFW(std::string( _sFilename.szString()) );
    oFW.OpenSection("Pose");
    for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
    {
        oFW.OpenSection(globals.m_poSkeleton->m_oBone[i].m_poBone->sGetName().szString());

            FEReal rAngle = _poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.oGetValue(_fAnimTime);
            oFW.AddVar("Angle",rAngle,DEF_ANGLE);

            oFW.OpenSection("Position");

                CFEVect2 oPos = _poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.oGetValue(_fAnimTime);
                oFW.AddVar("x",oPos.x,DEF_POS_X);
                oFW.AddVar("y",oPos.y,DEF_POS_Y);

            oFW.CloseSection();
        oFW.CloseSection();
    }
    oFW.CloseSection();
    oFW.Write();
}
//---------------------------------------------------------------------------
void LoadPoseFile(const CFEString& _sFilename,CSkeleton* _poSkeleton,float _fAnimTime)
{
    CFEConfigFile oFile( _sFilename );
    if (oFile.bInitialized())
    {
		for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
		{
			CFEString sVar = CFEString("Pose.") + globals.m_poSkeleton->m_oBone[i].m_poBone->sGetName();

			FEReal rAngle = _poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.oGetValue(_fAnimTime);
			CFEVect2 oPos = _poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.oGetValue(_fAnimTime);
			FEReal rOldAngle = rAngle;
			CFEVect2 oOldPos = oPos;

            rAngle  = oFile.rGetReal(sVar + CFEString(".Angle"),DEF_ANGLE);
			oPos.x  = oFile.rGetReal(sVar + CFEString(".Pos.x"),DEF_POS_X);
			oPos.y  = oFile.rGetReal(sVar + CFEString(".Pos.y"),DEF_POS_Y);

			// si existe el keyframe en ese momento, usar la funcion lerp de ese keyframe
			EFEKFLerpFunc eLerpFunc = KFLF_LERP;
			CFEKeyFrame* poKF = _poSkeleton->m_oBone[i].m_poPJointAction->m_oPosFunc.poGetKeyFrameAtTime(_fAnimTime);
			if (poKF!=NULL)
				eLerpFunc = poKF->eGetLerpFunc();
			
			if (rAngle != rOldAngle)
				_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.InsertKeyFrame(rAngle,_fAnimTime,eLerpFunc);
			if (oPos != oOldPos)
				_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.InsertKeyFrame(oPos,_fAnimTime,eLerpFunc);
		}
    }
}
//---------------------------------------------------------------------------
