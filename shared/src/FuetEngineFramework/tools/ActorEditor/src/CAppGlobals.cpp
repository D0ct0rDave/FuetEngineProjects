//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CAppGlobals.h"
#include "CActorEditorGUI.h"

#include "images/fkeyframe_icon.xpm"
#include "images/keyframe_icon.xpm"
#include "images/icon_bone.xpm"
#include "images/group_layer_icon.xpm"
#include "images/icon_layer_icon.xpm"
#include "images/icon_mesh.xpm"

#include "images/eye_grayed_icon.xpm"
#include "images/eye_icon.xpm"

#include "images/wrap_initialvalue_icon.xpm"
#include "images/wrap_finalvalue_icon.xpm"
#include "images/wrap_loop_icon.xpm"
#include "images/wrap_pingpong_icon.xpm"

#include "images/editor_icon_32x32.xpm"

//---------------------------------------------------------------------------
CAppGlobals globals;
float SCREEN_WIDTH;
float SCREEN_HEIGHT;
CFEVect2 INITIAL_AIMPOS;
//---------------------------------------------------------------------------
CAppColors::CAppColors()
{
	m_oGroupBV = CFEColor(0.25,1,0,0.75);
	m_oLayerBV = CFEColor(1,1,0,0.75);
    m_oLayerPivot = CFEColor(1,0,0,0.75);
    m_oGrid = CFEColor(1,1,1,0.25);
    m_oGridHL = CFEColor(1,1,1,0.5);
    m_oBackground = CFEColor(0,0,0,1);
    m_oFrameBackground = CFEColor(0,0,0.2,1);
    m_oFrameRect = CFEColor(0,0,1,1);
    m_oPrevFrameBackground = CFEColor(0.5,0.5,0.5,1);
    
    m_oBoneInPreview = CFEColor(1,0,1,1);
    
    m_oBone        = CFEColor(1,1,1,0.25);
    m_oBoneKF      = CFEColor(1,1,1,0.5);
    m_oSelBone     = CFEColor(1,1,0,1);
    m_oSelEditBone = CFEColor(0,1,0,1);
	m_oIniColor		= CFEColor(_025r,_025r,_025r,1);
    m_oCrossHairColor = CFEColor(1,0,1,0.35);

	m_oAppBack		= wxColor(0xc0,0xc0,0xc0);

	m_oGridLabelBase= wxColor(0xd6,0xd6,0xd6);
	m_oGridCellBase = wxColor(0xff,0xff,0xff);
	m_oGridCellText = wxColor(0x00,0x00,0x00);	

	m_oElemBack		= wxColor(255,250,229);
	m_oGenObjBack0	= wxColor(0xff,0xff,0xff);
	m_oGenObjBack1	= wxColor(0xf0,0xf0,0xf0);

	m_oSelCellBack	= wxColor(0x2e,0x54,0xf5);
	m_oSelCellText  = wxColor(0xff,0xff,0xff);
	
	// block selection
	m_oSelBlockFrame= wxColor(0x4f,0x4f,0xff);
	m_oSelBlockBack0= wxColor(0xcf,0xcf,0xff);
	m_oSelBlockBack1= wxColor(0xc0,0xc0,0xf0);
	m_oSelBlockText	= wxColor(0xff,0xff,0x00);

	m_oVisCellBack	= wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	m_oLockCellBack = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	m_oTLGridLine   = wxColor(0xc0,0xc0,0xc0);
	m_oTLDisabled	= wxColor(0xd6,0xd6,0xd6);
	m_oTLLineBack	= wxColor(0xd6,0xd6,0xd6);
	m_oTLMarkerLine	= wxColor(0xff,0x00,0x00);
	m_oTLMarkerBack	= wxColor(0xca,0x84,0x87);
	m_oTLThroughLine= wxColor(0x80,0x80,0xff);
	m_oTLTick		= wxColor(0x83,0x83,0x83);
	m_oTLTickNumber = wxColor(0x40,0x40,0x40);
	m_oTLLabels     = wxColor(0x40,0x40,0x40);
	m_oActionDefValBack = wxColor(0xff,0x99,0x55);
	m_oTLSelRow     = wxColor(0x00,0x00,0x55);
}
//---------------------------------------------------------------------------
CAppIcons::CAppIcons()
{
    m_oAppLogo = wxIcon(editor_icon_32x32_xpm);

    m_oShowLayer = wxBitmap(eye_icon);
    m_oHideLayer = wxBitmap(eye_grayed_icon);

    m_o1StKeyFrame = wxBitmap(fkeyframe_icon);
    m_oKeyFrame    = wxBitmap(keyframe_icon);

	m_oWrapInitialValue = wxBitmap(wrap_initialvalue_icon);
	m_oWrapFinalValue = wxBitmap(wrap_finalvalue_icon);
	m_oWrapLoop = wxBitmap(wrap_loop_icon);
	m_oWrapPingPong = wxBitmap(wrap_pingpong_icon);
	
	m_oGroupNode = wxBitmap(group_layer_icon_xpm);
	m_oSpriteNode = wxBitmap(icon_layer_icon_xpm);
    m_oBoneNode = wxBitmap(icon_bone);
    m_oMeshNode = wxBitmap(icon_mesh_xpm);
};
//---------------------------------------------------------------------------
CAppGlobals::CAppGlobals()
{
	m_poCopyPose = NULL;

    m_uiCurFrame = 0;
    m_fTime = 0.0f; 
    m_fGlobalTime = 0.0f;
    m_uiNumFrames = 0;

    m_sFrameFile = "";

    m_fTotalAnimTime = 1.0f;
    m_fCurAnimTime = 0.0;

    m_iSelBone = -1;
    
   	m_fZoom		 = INITIAL_ZOOM;
	m_oTrans	 = INITIAL_TRANS;
	m_oAimPos	 = INITIAL_AIMPOS;


    m_oIniCursorCoord.x = 0.0;
    m_oIniCursorCoord.y = 0.0;
    
    m_oCrossHair.x = 0.0;
    m_oCrossHair.y = 0.0;

    GUI = NULL;
    
    m_poSkeleton = NULL;
    // m_poPrevSkeleton = NULL;

	m_uiFrameBeforePlay = 0;

	CFEColor oColor;
	m_oBoneColor.InsertKeyFrame(m_oColorScheme.m_oBone,_0r,KFLF_LERP);
	oColor = m_oColorScheme.m_oBone * _025r; 
	oColor.a = m_oColorScheme.m_oBone.a;
	m_oBoneColor.InsertKeyFrame(oColor,COLOR_CYCLE_PERIOD,KFLF_LERP);
	m_oBoneColor.SetWrapMode(KFBFWM_PINGPONG);
	
	m_oBoneKFColor.InsertKeyFrame(m_oColorScheme.m_oBoneKF,_0r,KFLF_LERP);
	oColor = m_oColorScheme.m_oBoneKF * _025r; 
	oColor.a = m_oColorScheme.m_oBoneKF.a;
	m_oBoneKFColor.InsertKeyFrame(oColor,COLOR_CYCLE_PERIOD,KFLF_LERP);
	m_oBoneKFColor.SetWrapMode(KFBFWM_PINGPONG);
	
	m_oSelBoneColor.InsertKeyFrame(m_oColorScheme.m_oSelBone,_0r,KFLF_LERP);
	oColor = m_oColorScheme.m_oSelBone * _025r; 
	oColor.a = m_oColorScheme.m_oSelBone.a;
	m_oSelBoneColor.InsertKeyFrame(oColor,COLOR_CYCLE_PERIOD,KFLF_LERP);
	m_oSelBoneColor.SetWrapMode(KFBFWM_PINGPONG);

	m_oSelEditBoneColor.InsertKeyFrame(m_oColorScheme.m_oSelEditBone,_0r,KFLF_LERP);
	oColor = m_oColorScheme.m_oSelEditBone * _025r;
	oColor.a = m_oColorScheme.m_oSelEditBone.a;
	m_oSelEditBoneColor.InsertKeyFrame(oColor,COLOR_CYCLE_PERIOD,KFLF_LERP);
	m_oSelEditBoneColor.SetWrapMode(KFBFWM_PINGPONG);

    m_bRelaunchImage = false;

    // block selection
    m_uiBlockSelState	= 0;
    m_uiBlockStartFrame = 0;
    m_uiBlockEndFrame   = 0;
    m_uiBlockCurCol		= 0;
}
//---------------------------------------------------------------------------
CAppGlobals::~CAppGlobals()
{
	if (m_poCopyPose!=NULL)
	{
		delete m_poCopyPose;
		m_poCopyPose = NULL;
	}
	
	if (m_poSkeleton!=NULL)
	{
		delete m_poSkeleton;
		m_poSkeleton =NULL;
	}

	m_oUndoStack.TrimTo(0);
}
//---------------------------------------------------------------------------
int CAppGlobals::iGetKeyFrame(CBone* _poBone,uint _uiFrameTick,unsigned int _uiFlags)
{
	if (_uiFlags == 0) return(-1);

	// sanity checks
	if (_poBone == NULL) return(-1);
	float fFrameTime = _uiFrameTick*SECS_PER_TICK;

    // angle
    if (_uiFlags  & 0x01)
    {
		for (uint i=0;i<_poBone->m_poPJointAction->m_rAngleFunc.uiGetNumKeyFrames();i++)
		{
			CFEKeyFrame* poKF = _poBone->m_poPJointAction->m_rAngleFunc.poGetKeyFrame(i);

			if ((poKF != NULL) && (CFEMath::bBetween(fFrameTime-0.01f,fFrameTime+0.01f,poKF->rGetKeyTime())))
				return(i);
		}
	}
    // position
    if (_uiFlags  & 0x02)
    {
		for (uint i=0;i<_poBone->m_poBJointAction->m_oPosFunc.uiGetNumKeyFrames();i++)
		{
			CFEKeyFrame* poKF = _poBone->m_poBJointAction->m_oPosFunc.poGetKeyFrame(i);

			if ((poKF != NULL) && (CFEMath::bBetween(fFrameTime-0.01f,fFrameTime+0.01f,poKF->rGetKeyTime())))
				return(i);
		}
	}

	return(-1);
}
//---------------------------------------------------------------------------
EFEKFLerpFunc CAppGlobals::eGetLerpFunc(CBone* _poBone,uint _uiFrameTick)
{
	// sanity checks
	if (_poBone == NULL) return(KFLF_LERP);
	EFEKFLerpFunc eLF = KFLF_LERP;

    // position
	float fFrameTime = _uiFrameTick*SECS_PER_TICK;

    // position    
    for (uint i=0;i<_poBone->m_poPJointAction->m_rAngleFunc.uiGetNumKeyFrames();i++)
    {
        CFEKeyFrame* poKF = _poBone->m_poPJointAction->m_rAngleFunc.poGetKeyFrame(i);

        if ((poKF != NULL) && (CFEMath::bBetween(fFrameTime-0.01f,fFrameTime+0.01f,poKF->rGetKeyTime())))
			return(  poKF->eGetLerpFunc() );
    }    

	return(eLF);
}
//---------------------------------------------------------------------------
CSkelPose* CAppGlobals::poGetCurrentPose()
{
	CSkelPose* poPose = new CSkelPose;
                
	for (uint i=0;i<m_poSkeleton->m_oBone.size();i++)
	{
		poPose->m_rAngle.push_back( m_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.oGetValue(globals.m_fTime) );
		poPose->m_oPos.push_back  ( m_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.oGetValue(globals.m_fTime) );
	}

	return(poPose);
}
//---------------------------------------------------------------------------
void CAppGlobals::PastePose(CSkelPose* _poPose,bool _bForcedPaste)
{
	if (_poPose == NULL) return;

	// update instance just in case.
	CFESkelAnimInstMgr::I()->Update(globals.m_poSkeleton->m_hInst,_0r);

	for (uint i=0;i<m_poSkeleton->m_oBone.size();i++)
	{
		// si existe el keyframe en ese momento, usar la funcion lerp de ese keyframe
		EFEKFLerpFunc eLerpFunc = KFLF_LERP;
		CFEKeyFrame* poKF = m_poSkeleton->m_oBone[i].m_poPJointAction->m_oPosFunc.poGetKeyFrameAtTime(globals.m_fTime);
		if (poKF!=NULL) eLerpFunc = poKF->eGetLerpFunc();

		if (_bForcedPaste ==  false)
		{
			FEReal rOldAngle = m_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.oGetValue(globals.m_fTime);
			CFEVect2 oOldPos = m_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.oGetValue(globals.m_fTime);
			
			if (_poPose->m_rAngle[i] != rOldAngle)
				m_poSkeleton->OrientBone(i,_poPose->m_rAngle[i],globals.m_fTime);

			if (_poPose->m_oPos[i] != oOldPos)
				m_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.InsertKeyFrame(_poPose->m_oPos[i],globals.m_fTime,eLerpFunc);
		}
		else
		{
			m_poSkeleton->OrientBone(i,_poPose->m_rAngle[i],globals.m_fTime);
			m_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.InsertKeyFrame(_poPose->m_oPos[i],globals.m_fTime,eLerpFunc);
		}
	}
}
//---------------------------------------------------------------------------
bool bProcessedFunc(CFEArray<FEPointer>& _pAPF,FEPointer _pFunc)
{
	for (uint i=0;i<_pAPF.size();i++)
	{
		if (_pAPF[i] == _pFunc)
		{
			return(true);
		}
	}

	_pAPF.push_back(_pFunc);

	// 
	return(false);
}

template <class T>
inline void TrimKFBFunc(CFEKFBFunc<T>* _poFunc,FEReal _rTime)
{
	for (int k=_poFunc->uiGetNumKeyFrames()-1;k>=0;k--)
	{
		CFEKeyFrame* poKF = _poFunc->poGetKeyFrame(k);

		if (poKF->rGetKeyTime() > _rTime)
			_poFunc->DeleteKeyFrame(k);
	}
}
//---------------------------------------------------------------------------
void CAppGlobals::TrimAnimation(uint _uiFrameTick)
{
	CFEArray<FEPointer> m_pAPF; // already done funcs...
	
	// position
	float fFrameTime = (_uiFrameTick*SECS_PER_TICK)-EPSILON;

	for (uint b=0;b<m_poSkeleton->m_oBone.size();b++)
	{
		if (! bProcessedFunc(m_pAPF,(FEPointer)&m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc))
			TrimKFBFunc(&m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc,fFrameTime);

		if (! bProcessedFunc(m_pAPF,(FEPointer)&m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc))
			TrimKFBFunc(&m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc,fFrameTime);
	}
}
//---------------------------------------------------------------------------
// desplaza 1 tick a la derecha todos los key frames que se encuentran en la posición de tiempo indicada
// por el tick del parámetro de entrada
//---------------------------------------------------------------------------
template <class T>
inline void KFBFuncInsertFrameAt(CFEKFBFunc<T>* _poFunc,uint _uiTick,uint _uiTicksToInsert = 1);

template <class T>
inline void KFBFuncInsertFrameAt(CFEKFBFunc<T>* _poFunc,uint _uiTick,uint _uiTicksToInsert)
{
	for (uint k=0;k<_poFunc->uiGetNumKeyFrames();k++)
	{
		CFEKeyFrame* poKF = _poFunc->poGetKeyFrame(k);
		// get the tick this frame is placed in
		uint uiFrameTick = (poKF->rGetKeyTime()+EPSILON)/SECS_PER_TICK;
		// if the tick is greater or equal to the given one ...
		if (uiFrameTick > _uiTick)
		{
			float fFrameTime = ((uiFrameTick+_uiTicksToInsert)*SECS_PER_TICK);
			poKF->SetKeyTime(fFrameTime);
		}
	}
	
	_poFunc->Invalidate();
	_poFunc->RecalcMaxLimit();
}

void CAppGlobals::InsertFrameAt(uint _uiFrameTick)
{
	CFEArray<FEPointer> m_pAPF; // already done funcs...
	
	for (uint b=0;b<m_poSkeleton->m_oBone.size();b++)
	{
		if (! bProcessedFunc(m_pAPF,(FEPointer)&m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc))
			KFBFuncInsertFrameAt(&m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc,_uiFrameTick);
		if (! bProcessedFunc(m_pAPF,(FEPointer)&m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc))
			KFBFuncInsertFrameAt(&m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc,_uiFrameTick);
	}

	m_uiNumFrames++;
}
//---------------------------------------------------------------------------
// desplaza 1 tick a la izquierda todos los key frames que se encuentran en la posición de tiempo indicada
// por el tick del parámetro de entrada
//---------------------------------------------------------------------------
template <class T>
inline void KFBFuncDeleteFrameAt(CFEKFBFunc<T>* _poFunc,uint _uiTick,uint _uiTicksToDelete = 1);

template <class T>
inline void KFBFuncDeleteFrameAt(CFEKFBFunc<T>* _poFunc,uint _uiTick,uint _uiTicksToDelete)
{
	for (uint k=0;k<_poFunc->uiGetNumKeyFrames();k++)
	{
		CFEKeyFrame* poKF = _poFunc->poGetKeyFrame(k);
		// get the tick this frame is placed in
		uint uiFrameTick = (poKF->rGetKeyTime()+EPSILON)/SECS_PER_TICK;
		// if the tick is greater or equal to the given one ...
		if (uiFrameTick > _uiTick)
		{
			float fFrameTime = ((uiFrameTick-_uiTicksToDelete)*SECS_PER_TICK);
			poKF->SetKeyTime(fFrameTime);
		}
	}

	_poFunc->Invalidate();
	_poFunc->RecalcMaxLimit();
}

/// Deletes the frame in the given animation tick
void CAppGlobals::DeleteFrameAt(uint _uiFrameTick)
{
	if (_uiFrameTick==0) return;
	float fFrameTime = (_uiFrameTick*SECS_PER_TICK);

	CFEArray<FEPointer> m_pAPF; // already done funcs...
	
	for (uint b=0;b<globals.m_poSkeleton->m_oBone.size();b++)
	{
		CBone* poBone = &m_poSkeleton->m_oBone[b];
		
		if (! bProcessedFunc(m_pAPF,(FEPointer)&poBone->m_poPJointAction->m_oPosFunc))
		{
			poBone->m_poPJointAction->m_oPosFunc.DeleteKeyFrameAtTime(fFrameTime);
			KFBFuncDeleteFrameAt(&poBone->m_poPJointAction->m_oPosFunc,_uiFrameTick);
		}
		
		if (! bProcessedFunc(m_pAPF,(FEPointer)&poBone->m_poPJointAction->m_rAngleFunc))
		{
			poBone->m_poPJointAction->m_rAngleFunc.DeleteKeyFrameAtTime(fFrameTime);
			KFBFuncDeleteFrameAt(&poBone->m_poPJointAction->m_rAngleFunc,_uiFrameTick);
		}

		if (! bProcessedFunc(m_pAPF,(FEPointer)&poBone->m_poBJointAction->m_oPosFunc))
		{
			poBone->m_poBJointAction->m_oPosFunc.DeleteKeyFrameAtTime(fFrameTime);
			KFBFuncDeleteFrameAt(&poBone->m_poBJointAction->m_oPosFunc,_uiFrameTick);
		}

		if (! bProcessedFunc(m_pAPF,(FEPointer)&poBone->m_poBJointAction->m_rAngleFunc))
		{
			poBone->m_poBJointAction->m_rAngleFunc.DeleteKeyFrameAtTime(fFrameTime);
			KFBFuncDeleteFrameAt(&poBone->m_poBJointAction->m_rAngleFunc,_uiFrameTick);
		}
	}

	m_uiNumFrames--;
	if (m_uiCurFrame >= m_uiNumFrames)
		m_uiCurFrame = m_uiNumFrames-1;

	m_fTime = (float)m_uiCurFrame * SECS_PER_TICK;
}
//---------------------------------------------------------------------------
/// Deletes the frame in the given animation tick
void CAppGlobals::ClearFrameAt(uint _uiFrameTick)
{
	if (_uiFrameTick==0) return;
	float fFrameTime = (_uiFrameTick*SECS_PER_TICK);

	CFEArray<FEPointer> m_pAPF; // already done funcs...
	for (uint b=0;b<globals.m_poSkeleton->m_oBone.size();b++)
	{
		if (! bProcessedFunc(m_pAPF,(FEPointer)&m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc))
			globals.m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc.DeleteKeyFrameAtTime(fFrameTime);

		if (! bProcessedFunc(m_pAPF,(FEPointer)&m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc))
			globals.m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc.DeleteKeyFrameAtTime(fFrameTime);
	}
}
//---------------------------------------------------------------------------
void CAppGlobals::CopyKeyFramesFromTo(uint _uiSrcFrameTick,uint _uiDstFrameTick)
{
	FEReal rDstTime = _uiDstFrameTick*SECS_PER_TICK;
	for (uint b=0;b<globals.m_poSkeleton->m_oBone.size();b++)
	{
		int iKF = -1;
		
		iKF = iGetKeyFrame(globals.m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc,_uiSrcFrameTick);
		if (iKF != -1)
		{
			CFETypedKeyFrame<FEReal>* poKF = globals.m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc.poGetKeyFrameData(iKF);
			globals.m_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc.InsertKeyFrame(poKF->m_oKey,rDstTime,poKF->eGetLerpFunc());
		}
		
		iKF = iGetKeyFrame(globals.m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc,_uiSrcFrameTick);
		if (iKF != -1)
		{
			CFETypedKeyFrame<CFEVect2>* poKF = globals.m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc.poGetKeyFrameData(iKF);
			globals.m_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc.InsertKeyFrame(poKF->m_oKey,rDstTime,poKF->eGetLerpFunc());
		}
	}
}
//---------------------------------------------------------------------------
/// Fixes sprites to be synched with skeleton animation.
void CAppGlobals::SyncSprites(bool _bInvalidateBackups)
{
	// DMC: New Editor System
	float fTimeFactor = _1r;
    if (m_uiNumFrames>1)
		// fTimeFactor = (((m_uiNumFrames-1)*SECS_PER_TICK)/m_fTotalAnimTime);
		   fTimeFactor = (m_fTotalAnimTime/((m_uiNumFrames-1)*SECS_PER_TICK));

	globals.m_poSkeleton->SynchronizeSprites(fTimeFactor);

    /*
	for (uint b=0;b<globals.m_poSkeleton->m_oBone.size();b++)
    {
		if (globals.m_poSkeleton->m_oBone[b].m_poSpr != NULL)
		{
			FEHandler hSprInst = globals.m_poSkeleton->m_oBone[b].m_poSpr->hGetSprite();
			if (hSprInst == NULL) continue;

			CFESprite* poSprite = CFESpriteInstMgr::I()->poGetSprite(hSprInst);
			if (poSprite == NULL) continue;

			CFEString sSpriteName = CFESpriteMgr::I()->sGetResourceName(poSprite);
			
			CFEString sSpriteBack = sSpriteName + "_*back*";
			CFESprite* poSprBackup = NULL;

			if ((! CFESpriteMgr::I()->bExists(sSpriteBack)) || _bInvalidateBackups)
			{
				// if the resource exists release it
				if (CFESpriteMgr::I()->bExists(sSpriteName))
					CFESpriteMgr::I()->ReleaseResource(CFESpriteMgr::I()->poLoad(sSpriteBack));

				// create a backup version of the sprite
				poSprBackup = new CFESprite(*poSprite);
				CFESpriteMgr::I()->Register(poSprBackup,sSpriteBack);
			}
			else
			{
				poSprBackup = CFESpriteMgr::I()->poLoad(sSpriteBack);
			}
			
			// now fix animation times...
			for (uint a=0;a<poSprite->m_oActions.size();a++)
			{
				CFESpriteAction* poSpriteAction = poSprite->poGetAction(a);
				CFESpriteAction* poBackSpriteAction = poSprBackup->poGetAction(a);

				poSpriteAction->m_rActionTime = poBackSpriteAction->m_rActionTime * fTimeFactor;

				for (uint f=0;f<poSpriteAction->m_oSeq.size();f++)
				{
					poSpriteAction->m_oSeq[f].m_rBlendTime = poBackSpriteAction->m_oSeq[f].m_rBlendTime * fTimeFactor;
					poSpriteAction->m_oSeq[f].m_rDelay     = poBackSpriteAction->m_oSeq[f].m_rDelay     * fTimeFactor;
					poSpriteAction->m_oSeq[f].m_rFrameTime = poBackSpriteAction->m_oSeq[f].m_rFrameTime * fTimeFactor;
					poSpriteAction->m_oSeq[f].m_rStartTime = poBackSpriteAction->m_oSeq[f].m_rStartTime * fTimeFactor;
				}
			}
		}
    }
    */
}
//---------------------------------------------------------------------------
void CAppGlobals::SetKeyFrameFuncInterpolation(uint _uiBone,EFEKFLerpFunc _eLF)
{
	CBone* poBone = &globals.m_poSkeleton->m_oBone[_uiBone];

	for (uint k=0;k<poBone->m_poPJointAction->m_rAngleFunc.uiGetNumKeyFrames();k++)
	{
		CFEKeyFrame* poKF = poBone->m_poPJointAction->m_rAngleFunc.poGetKeyFrame(k);
		poKF->SetLerpFunc(_eLF);
	}
	
	for (uint k=0;k<poBone->m_poBJointAction->m_oPosFunc.uiGetNumKeyFrames();k++)
	{
		CFEKeyFrame* poKF = poBone->m_poBJointAction->m_oPosFunc.poGetKeyFrame(k);
		if (poKF != NULL)
			poKF->SetLerpFunc(_eLF);
	}
}
//---------------------------------------------------------------------------
void CAppGlobals::SetKeyFrameInterpolation(uint _uiBone,uint _uiKeyFrame,EFEKFLerpFunc _eLF)
{
	CBone* poBone = &globals.m_poSkeleton->m_oBone[_uiBone];
	
	int iKeyFrame = globals.iGetKeyFrame( poBone, _uiKeyFrame,0x01);
	if (iKeyFrame != -1)
	{
		CFEKeyFrame* poKF = poBone->m_poPJointAction->m_rAngleFunc.poGetKeyFrame(iKeyFrame);
		if (poKF != NULL)
			poKF->SetLerpFunc(_eLF);
	}

	iKeyFrame = globals.iGetKeyFrame( poBone, _uiKeyFrame,0x02);
	if (iKeyFrame != -1)
	{
		CFEKeyFrame* poKF = poBone->m_poBJointAction->m_oPosFunc.poGetKeyFrame(iKeyFrame);
		if (poKF != NULL)
			poKF->SetLerpFunc(_eLF);
	}
}
//---------------------------------------------------------------------------
// Coordinate conversion routines
CFEVect2 CAppGlobals::oScreen2CanvasCoords(const CFEVect2& _oPos)
{
	// punto en el "canvas"
	CFEVect2 oCanvasPos;
	oCanvasPos.x = (_oPos.x / globals.m_fZoom) + globals.m_oTrans.x;
	oCanvasPos.y = (_oPos.y / globals.m_fZoom) + globals.m_oTrans.y;
	return(oCanvasPos);
}
//---------------------------------------------------------------------------
CFEVect2 CAppGlobals::oCanvas2ScreenCoords(const CFEVect2& _oPos)
{
	// Punto en pantalla
	CFEVect2 oScreenPos;
	oScreenPos.x = (_oPos.x - globals.m_oTrans.x)*globals.m_fZoom;
	oScreenPos.y = (_oPos.y - globals.m_oTrans.y)*globals.m_fZoom;
	return(oScreenPos);
}
//---------------------------------------------------------------------------
class CBoneUndoState
{
	public:
		CFEVect2			m_oSkelPos;
		
		// position animation for this bone
		CFEKFBFunc<CFEVect2> m_oPosFunc;

		// angle animation for this bone
		CFEKFBFunc<FEReal>   m_rAngleFunc;
};

class CUndoState
{
	public:
		~CUndoState()
		{
			for (uint i=0;i<m_poBones.size();i++)
				delete m_poBones[i];
		}

		uint							m_uiNumFrames;
		uint							m_uiCurFrame;
        float							m_fTime;                // The current animation time

		int								m_iEditMode;
		CFEArray<CBoneUndoState*>		m_poBones;
};
//---------------------------------------------------------------------------
CUndoState* CAppGlobals::poGetUndoState()
{
	CUndoState* poUS = new CUndoState;
	
	// update instance with new values.
	for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
	{
		CBone* poBone = &m_poSkeleton->m_oBone[i];
		CBoneUndoState* poBoneUS = new CBoneUndoState;

		// copy skeleton position for this bone
		poBoneUS->m_oSkelPos = poBone->m_poBone->oGetPos();
		
		// retrieve whole animation for this bone.
		for (uint k=0;k<poBone->m_poPJointAction->m_rAngleFunc.uiGetNumKeyFrames();k++)
		{
			CFETypedKeyFrame<FEReal>* poKF = poBone->m_poPJointAction->m_rAngleFunc.poGetKeyFrameData(k);
			poBoneUS->m_rAngleFunc.InsertKeyFrame(poKF->m_oKey,poKF->rGetKeyTime(),poKF->eGetLerpFunc());
		}

		for (uint k=0;k<poBone->m_poBJointAction->m_oPosFunc.uiGetNumKeyFrames();k++)
		{
			CFETypedKeyFrame<CFEVect2>* poKF = poBone->m_poBJointAction->m_oPosFunc.poGetKeyFrameData(k);
			poBoneUS->m_oPosFunc.InsertKeyFrame(poKF->m_oKey,poKF->rGetKeyTime(),poKF->eGetLerpFunc());
		}
		
		poUS->m_poBones.push_back( poBoneUS );
	}

	poUS->m_fTime       = m_fTime;
	poUS->m_uiCurFrame  = m_uiCurFrame;
	poUS->m_uiNumFrames = m_uiNumFrames;

	return(poUS);
}
//---------------------------------------------------------------------------
void CAppGlobals::PasteUndoState(CUndoState* _poUS)
{
	// update instance with new values.
	for (uint i=0;i<_poUS->m_poBones.size();i++)
	{
		CBone* poBone = &m_poSkeleton->m_oBone[i];
		CBoneUndoState* poBoneUS = _poUS->m_poBones[i];

		// copy skeleton position for this bone
		poBone->m_poBone->SetPos( poBoneUS->m_oSkelPos );
		
		// retrieve whole animation for this bone.
		poBone->m_poPJointAction->m_rAngleFunc.Reset();
		for (uint k=0;k<poBoneUS->m_rAngleFunc.uiGetNumKeyFrames();k++)
		{
			CFETypedKeyFrame<FEReal>* poKF = poBoneUS->m_rAngleFunc.poGetKeyFrameData(k);
			poBone->m_poPJointAction->m_rAngleFunc.InsertKeyFrame(poKF->m_oKey,poKF->rGetKeyTime(),poKF->eGetLerpFunc());
		}

		poBone->m_poBJointAction->m_oPosFunc.Reset();
		for (uint k=0;k<poBoneUS->m_oPosFunc.uiGetNumKeyFrames();k++)
		{
			CFETypedKeyFrame<CFEVect2>* poKF = poBoneUS->m_oPosFunc.poGetKeyFrameData(k);
			poBone->m_poBJointAction->m_oPosFunc.InsertKeyFrame(poKF->m_oKey,poKF->rGetKeyTime(),poKF->eGetLerpFunc());
		}
	}

	// update instance with new values.
	uint uiCurAction = CFESkelAnimInstMgr::I()->uiGetAction(m_poSkeleton->m_hInst);
	CFESkelAnimInstMgr::I()->SetAction(m_poSkeleton->m_hInst,0);
	CFESkelAnimInstMgr::I()->Update(m_poSkeleton->m_hInst,_0r);
	CFESkelAnimInstMgr::I()->SetAction(m_poSkeleton->m_hInst,1);
	CFESkelAnimInstMgr::I()->Update(m_poSkeleton->m_hInst,_0r);
	CFESkelAnimInstMgr::I()->SetAction(m_poSkeleton->m_hInst,uiCurAction);     // WARNING: this puts the animation instance to the 0 sec pose
	CFESkelAnimInstMgr::I()->SetTime(m_poSkeleton->m_hInst,globals.m_fTime);
		
	extern void UpdateNodeInstance(CFESkelAnimNode* _poSrcNode);
	for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
	{
		if (globals.m_poSkeleton->m_oBone[i].m_poBone != NULL)
		{
			globals.m_poSkeleton->m_oBone[i].m_poBone->SetAngle(0);
			UpdateNodeInstance(globals.m_poSkeleton->m_oBone[i].m_poBone);
		}

		/*
		if (globals.m_poSkeleton->m_oBone[i].m_poBoneJoint != NULL)
		{
			globals.m_poSkeleton->m_oBone[i].m_poBoneJoint->SetAngle(0);
			globals.m_poSkeleton->m_oBone[i].m_poBoneJoint->SetPos(CFEVect2::ZERO());
			UpdateNodeInstance(globals.m_poSkeleton->m_oBone[i].m_poBoneJoint);
		}

		if (globals.m_poSkeleton->m_oBone[i].m_poChildJoint != NULL)
		{
			globals.m_poSkeleton->m_oBone[i].m_poChildJoint->SetAngle(0);
			globals.m_poSkeleton->m_oBone[i].m_poChildJoint->SetPos(CFEVect2::ZERO());
			UpdateNodeInstance(globals.m_poSkeleton->m_oBone[i].m_poChildJoint);
		}
		*/

		if (globals.m_poSkeleton->m_oBone[i].m_poParentJoint != NULL)
		{
			globals.m_poSkeleton->m_oBone[i].m_poParentJoint->SetAngle(0);
			globals.m_poSkeleton->m_oBone[i].m_poParentJoint->SetPos(CFEVect2::ZERO());
			UpdateNodeInstance(globals.m_poSkeleton->m_oBone[i].m_poParentJoint);
		}
	}

	m_fTime			= _poUS->m_fTime;
	m_uiCurFrame	= _poUS->m_uiCurFrame;
	m_uiNumFrames	= _poUS->m_uiNumFrames;
}
//---------------------------------------------------------------------------
void CAppGlobals::EditBegin()
{
	/*
	create skeleton
	clone skeleton
	push new skeleton into the stack
	
	or 
	
	create skeleton with copy constructor (param current skeleton)
	push old skeleton into the stack
	swap skeleton from globals.
	*/

	#ifndef DISABLE_UNDO_REDO
	CUndoState* poUS = poGetUndoState();
	m_oUndoStack.Push(poUS);
	m_oUndoStack.TrimTo(MAX_UNDO_STATES);

	globals.GUI->m_mbMainMenu->Enable(wxID_REDO,false);
	globals.GUI->m_mbMainMenu->Enable(wxID_UNDO,true);
	globals.GUI->m_btRedo->Disable();
	globals.GUI->m_btUndo->Enable();
	#endif
}
//---------------------------------------------------------------------------
void CAppGlobals::EditEnd()
{
	// Recalc action times
	CFESkelAnimAction* poAction = globals.m_poSkeleton->m_poAnim->poGetAnimActionSet()->poGetAction(0);
	poAction->SetActionTime( CFESkelAnimUtils::rGetActionTime(globals.m_poSkeleton->m_poAnim,poAction) );
	poAction->SetMaxActionTime( CFESkelAnimUtils::rGetMaxActionTime(globals.m_poSkeleton->m_poAnim,poAction) );

	// update instance with new values.
	CFESkelAnimInstMgr::I()->Update(m_poSkeleton->m_hInst,_0r);
}
//---------------------------------------------------------------------------
void CAppGlobals::EditUndo()
{
	CUndoState* poCurrentState = poGetUndoState();
	CUndoState* poUndoState = globals.m_oUndoStack.poPop(poCurrentState);
	PasteUndoState(poUndoState);

	delete poUndoState;
}

void CAppGlobals::EditRedo()
{
	CUndoState* poCurrentState = poGetUndoState();
	CUndoState* poUndoState = globals.m_oUndoStack.poUnpop(poCurrentState);
	PasteUndoState(poUndoState);

	delete poUndoState;
}
//---------------------------------------------------------------------------
