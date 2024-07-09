//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CFileOpsCommon.h"
#include "CActorEditorGUIFileOps.h"
#include "CActorEditorGUIFuncs.h"
#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
#include "edutils.h"
#include "CActorEditorGUI.h"
#include "wx/things/spinctld.h"
//-----------------------------------------------------------------------------
class CAnimFileNodeInfoWritter : public CFESkelAnimNodeVisitor
{
	protected:

		CConfigFileWriter*	m_poFW;
		CFEString			m_sWorkingDir;

	public:

		CAnimFileNodeInfoWritter() : m_poFW(NULL) {};

		static void Write(CFESkelAnimNode* _poNode,CConfigFileWriter* _poFW,const CFEString& _sWorkingDir)
		{
			CAnimFileNodeInfoWritter oObj;
			oObj.m_poFW = _poFW;
			oObj.m_sWorkingDir = _sWorkingDir;

			_poNode->Accept(&oObj);
		}

		/// 
		void WriteCommonProperties(CFESkelAnimNode* _poNode)
		{			
			m_poFW->AddVar("Name", _poNode->sGetName().szString(),"");

			
			m_poFW->AddVar("Angle", _poNode->rGetAngle(),DEF_ANGLE);

			// These properties are discardable since they are not edited by the application
			m_poFW->AddVar("Visible",(bool)_poNode->bIsVisible(),true);

			m_poFW->AddVar("Depth", _poNode->rGetDepth(),DEF_DEPTH,6);

			m_poFW->OpenSection("Pos");
				m_poFW->AddVar("x", _poNode->oGetPos().x,DEF_POS_X);
				m_poFW->AddVar("y", _poNode->oGetPos().y,DEF_POS_Y);
			m_poFW->CloseSection(); // pos

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

		/// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			m_poFW->OpenSection( _poObj->sGetName().szString() );
				WriteCommonProperties(_poObj);
			m_poFW->CloseSection();
			
			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

        /// 
		template <class T>
		inline void WriteNodeWithSprite(T* _poObj)
		{
			m_poFW->OpenSection( _poObj->sGetName().szString() );

				WriteCommonProperties(_poObj);

				CFEString sExternalFile = CFESpriteMgr::I()->sGetResourceName( CFESpriteInstMgr::I()->poGetSprite(_poObj->hGetSprite()));
				if (sExternalFile != "")
				{
					uint uiAction = _poObj->uiGetAction();

					
					CFEString sFilename = sRelativizePath( sExternalFile, m_sWorkingDir);
					CFEString sSprite = CFEStringUtils::sRemoveExtension( CFEStringUtils::sNormalizePath(sFilename) );

					m_poFW->AddVar("Sprite",sSprite.szString(),"");
					m_poFW->AddVar("SpriteAction",(int)uiAction,DEF_ACTION);
				}

			m_poFW->CloseSection();
		}
		
		/// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			WriteNodeWithSprite((CFESkelAnimSprite*)_poObj);
		}

        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			WriteNodeWithSprite((CFESkelAnimMesh*)_poObj);
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
			m_poFW->OpenSection( _poObj->sGetName().szString() );
				WriteCommonProperties(_poObj);
			m_poFW->CloseSection();

			if (_poObj->poGetChild(0) != NULL)
				_poObj->poGetChild(0)->Accept(this);
        }
};
//---------------------------------------------------------------------------
void SaveSkeletonDefinition(CSkeleton* _poSkeleton,CConfigFileWriter* _poFW,const CFEString& _sWorkingDir)
{
	_poFW->OpenSection("SkeletonDefinition");

		_poFW->AddVar("NumBones",_poSkeleton->m_oBone.size(),0);

		for (uint i=0;i<_poSkeleton->m_oBone.size();i++)
		{
			_poFW->OpenSection("Bone",i);

				_poFW->AddVar("Name",_poSkeleton->m_oBone[i].sGetName().szString(),"");
				_poFW->AddVar("Parent",_poSkeleton->m_oBone[i].m_sParentName.szString(),"");
				_poFW->AddVar("AffectParent",_poSkeleton->m_oBone[i].m_bAffectParent,true);

				_poFW->OpenSection("Position");

					_poFW->AddVar("x",_poSkeleton->m_oBone[i].m_poBone->oGetBonePos().x,DEF_POS_X);
					_poFW->AddVar("y",_poSkeleton->m_oBone[i].m_poBone->oGetBonePos().y,DEF_POS_Y);

				_poFW->CloseSection();

				#ifdef USE_SKELANIM_SCALE_COLOR
				_poFW->OpenSection("Scale");

					_poFW->AddVar("x",_poSkeleton->m_oBone[i].m_poBone->oGetScale().x,DEF_SCALE_X);
					_poFW->AddVar("y",_poSkeleton->m_oBone[i].m_poBone->oGetScale().y,DEF_SCALE_Y);					

				_poFW->CloseSection();
				#endif
				
				_poFW->AddVar("Angle",_poSkeleton->m_oBone[i].m_poBone->rGetAngle(),DEF_ANGLE);
				if (globals.m_poSkeleton->m_oBone[i].m_poLeafChild != NULL)
				{
					_poFW->OpenSection("Child");

						CSkeletonNodeWritter::Write(globals.m_poSkeleton->m_oBone[i].m_poLeafChild,_poFW,_sWorkingDir);

					_poFW->CloseSection();
				}

			_poFW->CloseSection();
		}

	_poFW->CloseSection();
}
//---------------------------------------------------------------------------
bool bSaveAnimation(const CFEString& _sFilename)
{
	// make backup of file
	if (bFileExists(_sFilename))	
		MakeBackupOfFile( wxString(_sFilename.szString()) );

    CLibConfigFileWriter oFW;
    if (! oFW.bCreate(std::string( _sFilename.szString()) ))
		return(false);

    CFEString sWorkingDir = CFEStringUtils::sGetPath( _sFilename );

    oFW.OpenSection("AnimEditor");

		CFEString sExtension = CFEStringUtils::sGetExtension(globals.m_sFrameFile);
		CFEString sFrameFilename = sRelativizePath( globals.m_sFrameFile, sWorkingDir) + sExtension; // sRelativizePath removes extension ...

        oFW.AddVar("FrameSequenceFile", sFrameFilename.szString(),"");
        oFW.AddVar("AnimTime", globals.m_fTotalAnimTime,DEF_ANIM_TIME);
        oFW.AddVar("Loop", globals.GUI->cb_Loop->GetValue(),true);
		oFW.AddVar("AnimatePivot",globals.GUI->m_cbAnimatePivot->GetValue(), false);
        oFW.AddVar("NumFrames",globals.m_uiNumFrames,(int)DEF_KEYFRAMES);

        // editor fields
        oFW.OpenSection("CamTrans");
            oFW.AddVar("x", globals.m_oTrans.x,DEF_POS_X);
            oFW.AddVar("y", globals.m_oTrans.y,DEF_POS_Y);
        oFW.CloseSection();

        oFW.AddVar("CamZoom", globals.m_fZoom,1.0f);
        oFW.AddVar("SelEditBone",globals.m_iSelEditBone,0);

        oFW.AddVar("ShowGridOnPreview",globals.GUI->cb_ShowGrid->GetValue(),true);
        oFW.AddVar("ShowBonesOnPreview",globals.GUI->cb_ShowPrevBones->GetValue(),true);
        oFW.AddVar("ShowLabels",globals.GUI->cb_ShowLabels->GetValue(),true);
        oFW.AddVar("ShowSprites",globals.GUI->cb_ShowSprites->GetValue(),true);
        
        oFW.AddVar("EditMode",globals.GUI->m_cbEditMode->GetSelection(),0);
		
		oFW.OpenSection("Background");
			oFW.AddVar("Show", globals.GUI->cb_ShowBackground->GetValue(),true);

			oFW.OpenSection("Scale");
				oFW.AddVar("x",globals.GUI->m_edBackXScale->GetValue(),_1r);
				oFW.AddVar("y",globals.GUI->m_edBackYScale->GetValue(),_1r);
			oFW.CloseSection();

			oFW.OpenSection("Offset");
				oFW.AddVar("x",globals.GUI->m_edBackXOffset->GetValue(),_0r);
				oFW.AddVar("y",globals.GUI->m_edBackYOffset->GetValue(),_0r);
			oFW.CloseSection();

			oFW.OpenSection("Color");
				oFW.AddVar("r",globals.m_oColorScheme.m_oFrameBackground.r,_1r);
				oFW.AddVar("g",globals.m_oColorScheme.m_oFrameBackground.g,_1r);
				oFW.AddVar("b",globals.m_oColorScheme.m_oFrameBackground.b,_1r);
			oFW.CloseSection();

		oFW.CloseSection();
        
        oFW.OpenSection("Crosshair");
            oFW.AddVar("x", globals.m_oCrossHair.x,DEF_POS_X);
            oFW.AddVar("y", globals.m_oCrossHair.y,DEF_POS_Y);
        oFW.CloseSection();
		
		// Write skeleton info
        SaveSkeletonDefinition(globals.m_poSkeleton,&oFW,sWorkingDir);

        // Write bone anims
        for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
        {
            oFW.OpenSection(globals.m_poSkeleton->m_oBone[i].sGetName().szString());
				
				oFW.AddVar("Visible", (bool)globals.m_poSkeleton->m_oBone[i].m_poPJointAction->bIsVisible(),DEF_VISIBILITY);

				// position
				oFW.OpenSection("PosFunc");

					WriteVectKeyFrames(globals.m_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc,&oFW,globals.GUI->cb_Loop->GetValue()?"Loop":"FinalValue");

				oFW.CloseSection();

				// angles
				oFW.OpenSection("AngleFunc");

					WriteFloatKeyFrames(globals.m_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc,&oFW,globals.GUI->cb_Loop->GetValue()?"Loop":"FinalValue");

				oFW.CloseSection();

            oFW.CloseSection();
        }

    oFW.CloseSection();
    oFW.Write();

	return(true);
}
//-----------------------------------------------------------------------------
class CAnimFileNodeInfoReader : public CFESkelAnimNodeVisitor
{
	protected:

		CFEConfigFile*		m_poCfg;

	public:

		CAnimFileNodeInfoReader() : m_poCfg(NULL) {};

		static void Read(CFESkelAnimNode* _poNode,CFEConfigFile* _poCfg)
		{
			CAnimFileNodeInfoReader oObj;
			oObj.m_poCfg		= _poCfg;

			_poNode->Accept(&oObj);
		}

		void ReadCommonProperties(CFESkelAnimNode* _poObj)
		{
			CFEString sVar = CFEString("AnimEditor.NodeInfo.") + _poObj->sGetName();

			_poObj->SetDepth( m_poCfg->rGetReal(sVar + ".Depth",DEF_DEPTH) );
			_poObj->SetAngle( m_poCfg->rGetReal(sVar + ".Angle",DEF_ANGLE) );

			_poObj->SetPos( CFEVect2(	m_poCfg->rGetReal(sVar + ".Position.x",DEF_POS_X),
										m_poCfg->rGetReal(sVar + ".Position.y",DEF_POS_Y)));
		
			#ifdef USE_SKELANIM_SCALE_COLOR
			_poObj->SetScale( CFEVect2(	m_poCfg->rGetReal(sVar + ".Scale.x",DEF_SCALE_X),
										m_poCfg->rGetReal(sVar + ".Scale.y",DEF_SCALE_Y)));

			_poObj->SetColor( CFEColor(	m_poCfg->rGetReal(sVar + ".Color.r",1.0f),
										m_poCfg->rGetReal(sVar + ".Color.g",1.0f),
										m_poCfg->rGetReal(sVar + ".Color.b",1.0f),
										m_poCfg->rGetReal(sVar + ".Color.a",1.0f)));
			#endif

			_poObj->Show( m_poCfg->bGetBool(sVar + ".Visible",true) );
		}

		/// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			ReadCommonProperties(_poObj);

			for (uint i=0;i<_poObj->uiNumChildren();i++)
			{
				if (_poObj->poGetChild(i) != NULL)
					_poObj->poGetChild(i)->Accept(this);
			}
        }

        template <class T>
		inline void ReadNodeWithSprite(T* _poObj)
		{
			CFEString sVar = CFEString("AnimEditor.NodeInfo.") + _poObj->sGetName();
			ReadCommonProperties(_poObj);

			CFEString sExternalFile	= m_poCfg->sGetString ( sVar + ".Sprite","");
			int iSpriteAction		= m_poCfg->iGetInteger( sVar + ".SpriteAction",DEF_ACTION);

			FEHandler hSprInst = NULL;
			if (sExternalFile != "")
			{
				hSprInst = CFESpriteInstMgr::I()->hGetInstance( sExternalFile );
				if (hSprInst != NULL)
				{
					_poObj->SetSprite( hSprInst );
					_poObj->SetAction( iSpriteAction );
				}
			}
		}
		
		/// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			ReadNodeWithSprite((CFESkelAnimSprite*)_poObj);
		}		

		/// 
        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			ReadNodeWithSprite((CFESkelAnimMesh*)_poObj);
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
			ReadCommonProperties(_poObj);
			
			if (_poObj->poGetChild(0)!=NULL)
				_poObj->poGetChild(0)->Accept(this);
        }
};
//---------------------------------------------------------------------------
bool bLoadAnimation(const CFEString& _sFilename)
{
    CFEConfigFile oFile( _sFilename.szString() );
    if (! oFile.bInitialized()) return(false);
    
    CFESkelAnimInstMgr::I()->ReleaseInstance(globals.m_poSkeleton->m_hInst);
    delete globals.m_poSkeleton;

    globals.m_sFrameFile = oFile.sGetString("AnimEditor.FrameSequenceFile","");
    if (! bLoadFrameSequence( globals.m_sFrameFile))
    {
        
    }

    globals.m_uiNumFrames = oFile.iGetInteger("AnimEditor.NumFrames",globals.m_oFrames.size());

    globals.m_uiCurFrame = 0;                            
    globals.m_fTime		 = _0r;
    globals.m_fGlobalTime= _0r;
    globals.m_fTotalAnimTime = oFile.rGetReal("AnimEditor.AnimTime",DEF_ANIM_TIME);
    globals.GUI->cb_Loop->SetValue( oFile.bGetBool("AnimEditor.Loop",true) );
	globals.GUI->m_cbAnimatePivot->SetValue( oFile.bGetBool("AnimEditor.AnimatePivot", false) );

    // editor fields
    globals.m_oTrans.x = oFile.rGetReal("AnimEditor.CamTrans.x", DEF_POS_X);
    globals.m_oTrans.y = oFile.rGetReal("AnimEditor.CamTrans.y", DEF_POS_Y);
    globals.m_fZoom  = oFile.rGetReal("AnimEditor.CamZoom", 1.0  );

    globals.GUI->cb_ShowPrevBones->SetValue( oFile.bGetBool("AnimEditor.ShowBonesOnPreview", true) );
    globals.GUI->cb_ShowGrid->SetValue( oFile.bGetBool("AnimEditor.ShowGridOnPreview",true) );
    globals.GUI->cb_ShowLabels->SetValue( oFile.bGetBool("AnimEditor.ShowLabels", true) );
    globals.GUI->cb_ShowSprites->SetValue( oFile.bGetBool("AnimEditor.ShowSprites", true) );
    
    globals.GUI->m_cbEditMode->SetSelection(oFile.iGetInteger("AnimEditor.EditMode",0) );
    
    globals.GUI->cb_ShowBackground->SetValue( oFile.bGetBool("AnimEditor.Background.Show", true) );
    globals.GUI->m_edBackXScale->SetValue(oFile.rGetReal("AnimEditor.Background.Scale.x",_1r) );
    globals.GUI->m_edBackYScale->SetValue(oFile.rGetReal("AnimEditor.Background.Scale.y",_1r) );
    globals.GUI->m_edBackXOffset->SetValue(oFile.rGetReal("AnimEditor.Background.Offset.x",_0r) );
    globals.GUI->m_edBackYOffset->SetValue(oFile.rGetReal("AnimEditor.Background.Offset.y",_0r) );
	globals.m_oColorScheme.m_oFrameBackground.r = oFile.rGetReal("AnimEditor.Background.Color.r",globals.m_oColorScheme.m_oFrameBackground.r);
	globals.m_oColorScheme.m_oFrameBackground.g = oFile.rGetReal("AnimEditor.Background.Color.g",globals.m_oColorScheme.m_oFrameBackground.g);
	globals.m_oColorScheme.m_oFrameBackground.b = oFile.rGetReal("AnimEditor.Background.Color.b",globals.m_oColorScheme.m_oFrameBackground.b);

    globals.m_oCrossHair.x = oFile.rGetReal("AnimEditor.CrossHair.x", DEF_POS_X  );
    globals.m_oCrossHair.y = oFile.rGetReal("AnimEditor.CrossHair.y", DEF_POS_Y  );

	// Load skeleton info
    globals.m_poSkeleton = new CSkeleton(oFile,"AnimEditor.");
    globals.m_poSkeleton->SetEditMode(globals.GUI->m_cbEditMode->GetSelection());
	CFESkelAnimInstMgr::I()->SetAction( globals.m_poSkeleton->m_hInst, globals.GUI->m_cbEditMode->GetSelection() );	// WARNING: this puts the animation instance to the 0 sec pose

    // globals.m_poSkeleton->RescaleSkeleton(SCREEN_VWIDTH,SCREEN_VHEIGHT);
	globals.m_iSelEditBone = oFile.iGetInteger("AnimEditor.SelEditBone",0);
	if (globals.m_iSelEditBone >= globals.m_poSkeleton->m_oBone.size())
		globals.m_iSelEditBone = globals.m_poSkeleton->m_oBone.size()-1;

	// -------------
    // read bone animation
    // -------------
    for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
    {
		CBone* poBone = &globals.m_poSkeleton->m_oBone[i];

        // delete the current contents.
        poBone->m_poBJointAction->m_oPosFunc.Reset();
        poBone->m_poBJointAction->m_oPosFunc.InsertKeyFrame(CFEVect2::ZERO(),0.0f,KFLF_LERP); // add a default value
        poBone->m_poPJointAction->m_rAngleFunc.Reset();
        poBone->m_poPJointAction->m_rAngleFunc.InsertKeyFrame(_0r,0.0f,KFLF_LERP); // add a default value

        CFEString sBoneVar = CFEString("AnimEditor.") + poBone->sGetName();

		bool bVisible = oFile.bGetBool(sBoneVar + CFEString(".Visible"),true);
		
		// -------------
		// add position key frames
		// -------------
		CFEString sVar      = CFEString("AnimEditor.") + CFEString( poBone->sGetName() ) + CFEString(".PosFunc");
		EFEKFBFuncWrapMode eWrapMode = KFBFWM_FINALVALUE; // CFEKFBFuncUtils::eGetWrapMode( oFile.sGetString(sVar+".WrapMode",DEF_FEWRAP) );
		uint uiNumKeyFrames = oFile.iGetInteger(sVar+".NumKeyFrames",DEF_KEYFRAMES);
		
		poBone->m_poBJointAction->Show(bVisible);
		poBone->m_poBJointAction->m_oPosFunc.SetWrapMode( eWrapMode );

		for (uint k=0;k<uiNumKeyFrames;k++)
		{
			CFEString sKVar = sVar + CFEString(".KeyFrame") + CFEString((int)k);
			CFEVect2 oPos;

			oPos.x = oFile.rGetReal(sKVar + CFEString(".x"), DEF_POS_X);
			oPos.y = oFile.rGetReal(sKVar + CFEString(".y"), DEF_POS_Y);
			FEReal rTime = oFile.rGetReal(sKVar + CFEString(".Time"), DEF_TIME);
			EFEKFLerpFunc eLerpFunc = CFEKFBFuncUtils::eGetLerpFunc( oFile.sGetString(sKVar+".LerpFunc",DEF_FELERP) );

			poBone->m_poBJointAction->m_oPosFunc.InsertKeyFrame(oPos,rTime,eLerpFunc);
		}

		// -------------
		// add angle key frames
		// -------------
		sVar      = CFEString("AnimEditor.") + CFEString( poBone->sGetName() ) + CFEString(".AngleFunc");
		eWrapMode = KFBFWM_FINALVALUE; // CFEKFBFuncUtils::eGetWrapMode( oFile.sGetString(sVar+".WrapMode",DEF_FEWRAP) );
		uiNumKeyFrames = oFile.iGetInteger(sVar+".NumKeyFrames",DEF_KEYFRAMES);

		poBone->m_poPJointAction->Show(bVisible);
		poBone->m_poPJointAction->m_oPosFunc.SetWrapMode( eWrapMode );

		for (uint k=0;k<uiNumKeyFrames;k++)
		{
			CFEString sKVar = sVar + CFEString(".KeyFrame") + CFEString((int)k);
			FEReal rVal= oFile.rGetReal(sKVar + CFEString(".Value"), DEF_ANGLE);
			FEReal rTime = oFile.rGetReal(sKVar + CFEString(".Time"), DEF_TIME);
			EFEKFLerpFunc eLerpFunc = CFEKFBFuncUtils::eGetLerpFunc( oFile.sGetString(sKVar+".LerpFunc",DEF_FELERP) );

			poBone->m_poPJointAction->m_rAngleFunc.InsertKeyFrame(rVal,rTime,eLerpFunc);
		}
	}

	// Recalc action times
	CFESkelAnimAction* poAction = globals.m_poSkeleton->m_poAnim->poGetAnimActionSet()->poGetAction(0);
	poAction->SetActionTime(CFESkelAnimUtils::rGetActionTime(globals.m_poSkeleton->m_poAnim,poAction));
	poAction->SetMaxActionTime( CFESkelAnimUtils::rGetMaxActionTime(globals.m_poSkeleton->m_poAnim ,poAction));
		
	globals.SyncSprites();
    return(true);
}
//---------------------------------------------------------------------------
