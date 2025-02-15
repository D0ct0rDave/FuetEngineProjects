//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CKFRMActorEditorGUIFileOps.h"
#include "CKFRMActorEditorGUIFuncs.h"

#include <vector>

#ifdef WIN32
#include <windows.h>
#endif

#include <wx/string.h>

#include <FreeImage.h>
#include <FreeImagePlus.h>

#include <FuetEngine.h>
#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
// ----------------------------------------------------------------------------
std::string sGetLerpFunc(EFEKFLerpFunc _eLerpFunc)
{
    switch(_eLerpFunc)
    {
        case KFLF_CONSTANT:
	    return("constant");
	    break;
    	
	    case KFLF_LERP:
	    return("linear");
	    break;

	    case KFLF_SIN:
	    return("sin");
	    break;

	    case KFLF_EXP:
	    return("exp");
	    break;

	    case KFLF_RAND:
	    return("random");
	    break;

	    case KFLF_SINSIN:
	    return("sinsin");
	    break;

	    case KFLF_EXPLOG:
	    return("explog");
	    break;
	}
	
	return("linear");
}
//-----------------------------------------------------------------------------
static EFEKFBFuncWrapMode eGetWrapMode(const CFEString& _sWrapMode)
{
    if (_sWrapMode |= "Loop")
        return(KFBFWM_LOOP);

else if (_sWrapMode |= "PingPong")
        return(KFBFWM_PINGPONG);

else if (_sWrapMode |= "InitialValue")
        return(KFBFWM_INITIALVALUE);

else if (_sWrapMode |= "FinalValue")
        return(KFBFWM_FINALVALUE);

	return(KFBFWM_FINALVALUE);
}
//-----------------------------------------------------------------------------
static EFEKFLerpFunc eGetLerpFunc(const CFEString& _sLerpFunc)
{
    if (_sLerpFunc |= "constant")
        return(KFLF_CONSTANT);

else if (_sLerpFunc |= "sin")
        return(KFLF_SIN);

else if (_sLerpFunc |= "exp")
        return(KFLF_EXP);

else if (_sLerpFunc |= "random")
        return(KFLF_RAND);

else if (_sLerpFunc |= "linear")
        return(KFLF_LERP);

else if (_sLerpFunc |= "sinsin")
        return(KFLF_SINSIN);

else if (_sLerpFunc |= "explog")
        return(KFLF_EXPLOG);
        
	return(KFLF_LERP);
}
//-----------------------------------------------------------------------------
void WriteFloatKeyFrames(CFEKFBFunc<float>& _oCoords,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode)
{
	_poFWriter->AddVar("WrapMode",_sWrapMode);
    _poFWriter->AddVar("NumKeyFrames",_oCoords.uiGetNumKeyFrames());

    for (uint uiKeyFrame=0;uiKeyFrame<_oCoords.uiGetNumKeyFrames();uiKeyFrame++)
    {
        CFEKeyFrame* poKF = _oCoords.poGetKeyFrame(uiKeyFrame);

        std::string sLF = sGetLerpFunc(poKF->eGetLerpFunc());
        FEReal rVal     = _oCoords.oGetValue( poKF->rGetKeyTime() );

        _poFWriter->OpenSection("KeyFrame",uiKeyFrame);

            _poFWriter->AddVar("Value", rVal);
            _poFWriter->AddVar("Time",poKF->rGetKeyTime());
            _poFWriter->AddVar("LerpFunc", sLF );

        _poFWriter->CloseSection();
    };
}
//-----------------------------------------------------------------------------
void WriteVectKeyFrames(CFEKFBFunc<CFEVect2>& _oCoords,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode)
{
	_poFWriter->AddVar("WrapMode",_sWrapMode);
    _poFWriter->AddVar("NumKeyFrames",_oCoords.uiGetNumKeyFrames());

    for (uint uiKeyFrame=0;uiKeyFrame<_oCoords.uiGetNumKeyFrames();uiKeyFrame++)
    {
        CFETypedKeyFrame<CFEVect2>* poKF = _oCoords.poGetKeyFrameData(uiKeyFrame);

        std::string sLF = sGetLerpFunc(poKF->eGetLerpFunc());
        CFEVect2 oPos = poKF->m_oKey;

        _poFWriter->OpenSection("KeyFrame",uiKeyFrame);

            _poFWriter->AddVar("x", oPos.x);
            _poFWriter->AddVar("y", oPos.y);
            _poFWriter->AddVar("Time",poKF->rGetKeyTime());
            _poFWriter->AddVar("LerpFunc", sLF );

        _poFWriter->CloseSection();
    };
}
//---------------------------------------------------------------------------
const uint BONE_ORDER[] = {

    // first left part
    BONE_LHIP,
    BONE_LKNEE,
    BONE_LFOOT,

    BONE_LSHOULDER,
    BONE_LELBOW,
    BONE_LHAND,

    // then body and head
    BONE_HIP,
    BONE_NECK,

    // first right part
    BONE_RHIP,
    BONE_RKNEE,
    BONE_RFOOT,

    BONE_RSHOULDER,
    BONE_RELBOW,
    BONE_RHAND,
};
//---------------------------------------------------------------------------
void SaveSkeleton(const CFEString& _sFilename,CSkeleton* _poSkeleton)
{
    CFEString sWorkingDir = CFEStringUtils::sGetPath( _sFilename );
    CLibConfigFileWriter oFW(std::string( _sFilename.szString()) );
    oFW.OpenSection("SkelNode");

        oFW.AddVar("Name","skeleton");
        oFW.AddVar("Type","group");
        oFW.AddVar("NumNodes",NUM_NORMAL_BONES+4);

        // write bones
        uint uiNumBones = 0;
        uint b;
        for (b=0;b<NUM_NORMAL_BONES;b++)
        {
            uint i=BONE_ORDER[b];
            
            /// add mesh first (if any)                
            if (
                   (_poSkeleton->m_oBone[ i ].m_poMesh != NULL) 
                &&
                    (
                           (i == BONE_RSHOULDER) 
                        || (i == BONE_LSHOULDER) 
                        || (i == BONE_RHIP) 
                        || (i == BONE_LHIP)
                    )
                )
            {
                oFW.OpenSection("Node",uiNumBones);
                
                    oFW.AddVar("Type","mesh");
                    oFW.AddVar("Name", _poSkeleton->m_oBone[ i ].m_poMesh->sGetName().szString() );

                    CFEString sFilename = CFEStringUtils::sGetFilename( CFEMaterialMgr::sGetResourceName( (FEHandler*)_poSkeleton->m_oBone[ i ].m_poMesh->hGetMaterial()) );
                    sFilename = CFEStringUtils::sRemoveExtension( sFilename );
                    oFW.AddVar("Material", sFilename.szString() );
                    oFW.AddVar("NumAttachedBones", _poSkeleton->m_oBone[ i ].m_poMesh->uiGetNumAttachedBones() );

                    for (uint ab = 0;ab<_poSkeleton->m_oBone[ i ].m_poMesh->uiGetNumAttachedBones();ab++)
                    {
                        oFW.OpenSection("AttachedBone",ab);
                            oFW.AddVar("Name",_poSkeleton->m_oBone[ i ].m_poMesh->poGetAttachedBone(ab)->sGetName().szString() );
                        oFW.CloseSection();
                    }

                oFW.CloseSection();
                uiNumBones++;
            }

            // add bone
            oFW.OpenSection("Node",uiNumBones);

                oFW.AddVar("Type","bone");
                oFW.AddVar("Name", _poSkeleton->m_oBone[ i ].m_poNode->sGetName().szString() );

                oFW.OpenSection("Scale");
                    oFW.AddVar("x", _poSkeleton->m_oBone[ i ].m_poNode->oGetScale().x );
                    oFW.AddVar("y", _poSkeleton->m_oBone[ i ].m_poNode->oGetScale().y );
                oFW.CloseSection(); // scale

                if (_poSkeleton->m_oBone[ i ].m_poSpr != NULL)                
                {
                    oFW.OpenSection("AttachedNode");

                        oFW.AddVar("Type","sprite");
                        oFW.AddVar("Name",_poSkeleton->m_oBone[ i ].m_poSpr->sGetName().szString() );

                        if (_poSkeleton->m_oBone[ i ].m_sExternalFile != "")
                        {
                            CFEString sFilename = CFEStringUtils::sGetFilename( _poSkeleton->m_oBone[ i ].m_sExternalFile );
                            sFilename = CFEStringUtils::sRemoveExtension( sFilename );
                            oFW.AddVar("Sprite",sFilename.szString() );
                        }

                        if ((_poSkeleton->m_oBone[ i ].m_oOfs.x != 0.0f) || (_poSkeleton->m_oBone[ i ].m_oOfs.y != 0.0f))
                        {
                            oFW.OpenSection("Position");
                                oFW.AddVar("x",_poSkeleton->m_oBone[ i ].m_oOfs.x);
                                oFW.AddVar("y",_poSkeleton->m_oBone[ i ].m_oOfs.y);
                            oFW.CloseSection();
                        }

                    oFW.CloseSection(); //  AttachedNode
                }

            oFW.CloseSection(); // Node(Bone)
            uiNumBones++;                
        }

    oFW.CloseSection(); // SkelNode
    oFW.Write();
}
//---------------------------------------------------------------------------
void LoadSkeleton(const CFEString& _sFilename,CSkeleton* _poSkeleton)
{
    /*
    // TODO: Reimplement Load Skeleton function to work properly ...
    CFEString sFilename = CFEStringUtils::sRemoveExtension(_sFilename);
    CFESkelAnimNode* poNode = CFESkelAnimLoader::poLoadNode(sFilename);

    if (poNode != NULL)
    {
        for (uint i=0;i<MAX_BONES;i++)
        {
            CFESkelAnimBone* poBone = (CFESkelAnimBone*)CFESkelAnimNodeLocator::poLocateNode( poNode,_poSkeleton->m_oBone[i].m_poNode->sGetName() );                
            if (poBone!=NULL)
            {
                _poSkeleton->m_oBone[i].m_poNode->SetScale( poBone->oGetScale() );
                globals.m_fHScale = poBone->oGetScale().x / 640.0f;
                globals.m_fVScale = poBone->oGetScale().y / 480.0f;
            }

            if (_poSkeleton->m_oBone[i].m_poMesh != NULL)
            {
                CFESkelAnimMeshModel* poMeshModel = (CFESkelAnimMeshModel*)CFESkelAnimNodeLocator::poLocateNode(poNode,_poSkeleton->m_oBone[i].m_poMesh->sGetName());
                if (poMeshModel!=NULL)
                {                    // Assign material from loaded skeleton to skeleton.
                    CFEString sMat = poMeshModel->sGetMaterial();
                    FEHandler hMat = CFEMaterialMgr::hLoad(sMat);

                    if (hMat != NULL)
                        _poSkeleton->m_oBone[i].m_poMesh->SetMaterial( hMat );
                }
            }

       else if (_poSkeleton->m_oBone[i].m_poSpr != NULL)
            {
                CFESkelAnimSpriteModel* poSpriteModel = (CFESkelAnimSpriteModel*)CFESkelAnimNodeLocator::poLocateNode(poNode,_poSkeleton->m_oBone[i].m_poSpr->sGetName());
                if (poSpriteModel!=NULL)
                {
                    // Assign material from loaded skeleton to skeleton.
                    CFEString sSprite = poSpriteModel->sGetSprite();
                    FEHandler hSprite = CFESpriteMgr::poLoad(sSprite);

                    if (hSprite != NULL)
                        _poSkeleton->m_oBone[i].m_poSpr->SetSprite( hSprite );

                    _poSkeleton->m_oBone[i].m_poSpr->SetPos( poSpriteModel->oGetPos() ); 
                    _poSkeleton->m_oBone[i].m_oOfs = poSpriteModel->oGetPos();
                }
            }
        }
    }
    */
}
//---------------------------------------------------------------------------
