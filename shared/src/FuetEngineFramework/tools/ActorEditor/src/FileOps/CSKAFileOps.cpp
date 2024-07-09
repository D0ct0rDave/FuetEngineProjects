//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CFileOpsCommon.h"
#include "CSKAFileOps.h"
#include "edutils.h"

#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
#include "CActorEditorGUI.h"
//---------------------------------------------------------------------------
static CUndoState* gpoUndoState = NULL;
//---------------------------------------------------------------------------
void ActionsPresaveProcess(CSkeleton *_poSkeleton,bool _bAnimPivot)
{
    // Save the current state.
    gpoUndoState = globals.poGetUndoState();

	if (_bAnimPivot == false)
	{
		// Set pivot position function to 0
		_poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBJointAction->m_oPosFunc.Reset();
		_poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBJointAction->m_oPosFunc.InsertKeyFrame(CFEVect2::ZERO(),_0r,KFLF_LERP);
	}
	else
	{
		// Substract skeleton bone position.
		CFEVect2 oPivotPos = _poSkeleton->oGetAbsBonePos(_poSkeleton->m_uiPivotIdx,_0r,0);

        for (uint k=0;k<_poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBJointAction->m_oPosFunc.uiGetNumKeyFrames();k++)
        {
	        CFETypedKeyFrame<CFEVect2>* poKF = _poSkeleton->m_oBone[_poSkeleton->m_uiPivotIdx].m_poBJointAction->m_oPosFunc.poGetKeyFrameData(k);
	        poKF->m_oKey -= oPivotPos;
		}
	}

    // Scale up time
    float fTimeFactor = _1r;
    if (globals.m_uiNumFrames>1)
		fTimeFactor = (globals.m_fTotalAnimTime/((globals.m_uiNumFrames-1)*SECS_PER_TICK));

    for (uint b=0;b<_poSkeleton->m_oBone.size();b++)
    {
		CFEKeyFrame* poKF = NULL;

        for (uint k=0;k<_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc.uiGetNumKeyFrames();k++)
        {
            poKF = _poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc.poGetKeyFrame(k);
	        poKF->SetKeyTime( poKF->rGetKeyTime() * fTimeFactor );
        }

        for (uint k=0;k<_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc.uiGetNumKeyFrames();k++)
        {
            poKF = _poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc.poGetKeyFrame(k);
            poKF->SetKeyTime( poKF->rGetKeyTime() * fTimeFactor );
        }
    }
}
//---------------------------------------------------------------------------
void ActionsPostsaveProcess(CSkeleton *_poSkeleton)
{
    // Undo changes
    globals.PasteUndoState(gpoUndoState);
}
//---------------------------------------------------------------------------
class CNodeActionWritter : public CFESkelAnimNodeVisitor
{
	protected:

		CConfigFileWriter*	m_poFW;
		uint				m_uiActionIdx;
		bool				m_b1LevelBones;
		bool				m_bBoneVisited;

	public:

		CNodeActionWritter() : m_poFW(NULL) {};

		static uint Write(CFESkelAnimNode* _poNode,CConfigFileWriter* _poFW,uint _uiStartActionIdx,bool _b1LevelBones = false)
		{
			CNodeActionWritter oObj;

			oObj.m_poFW			= _poFW;
			oObj.m_uiActionIdx  = _uiStartActionIdx;
			oObj.m_bBoneVisited	= false;
			oObj.m_b1LevelBones = _b1LevelBones;

			_poNode->Accept(&oObj);
			
			return(oObj.m_uiActionIdx);
		}

		void WriteCommonProperties(CFESkelAnimNode* _poNode)
		{			
			m_poFW->AddVar("Name", _poNode->sGetName().szString(),"");

			#ifdef USE_SKELANIM_SCALE_COLOR
			m_poFW->OpenSection("Scale");
				m_poFW->AddVar("x", _poNode->oGetScale().x,DEF_SCALE_X);
				m_poFW->AddVar("y", _poNode->oGetScale().y,DEF_SCALE_Y);
			m_poFW->CloseSection(); // scale
			#endif

			m_poFW->AddVar("Angle", _poNode->rGetAngle(),DEF_ANGLE);

			#if 0
			// These properties are discardable since they are not edited by the application
			m_poFW->AddVar("Visible", _poNode->bIsVisible(),true);

			m_poFW->AddVar("Depth", _poNode->rGetDepth(),DEF_DEPTH);

			m_poFW->OpenSection("Pos");
				m_poFW->AddVar("x", _poNode->oGetPos().x,DEF_POS_X);
				m_poFW->AddVar("y", _poNode->oGetPos().y,DEF_POS_Y);
			m_poFW->CloseSection(); // pos

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
			if (! _poObj->bIsVisible())
			{
				m_poFW->OpenSection("NodeAction",m_uiActionIdx);
					m_poFW->AddVar("NodeName", _poObj->sGetName().szString(),"" );
					m_poFW->AddVar("Visible", false, DEF_VISIBILITY);	
				m_poFW->CloseSection();

				m_uiActionIdx++;
			}
			else
			{
				// go down the hierarchy
				for (uint i=0;i<_poObj->uiNumChildren();i++)
					if (_poObj->poGetChild(i) != NULL)
						_poObj->poGetChild(i)->Accept(this);
			}	
        }

		template <class T>
		inline void WriteNodeWithSprite(T* _poObj)
		{
			// If there is no specific action to do with this sprite just return.
			if (_poObj->bIsVisible() && (_poObj->uiGetAction() == DEF_ACTION)) return;

			m_poFW->OpenSection("NodeAction",m_uiActionIdx);
				m_poFW->AddVar("NodeName", _poObj->sGetName().szString(),"" );

				if (! _poObj->bIsVisible())
				{
					m_poFW->AddVar("Visible", false, DEF_VISIBILITY);
				}
				else
				{
					m_poFW->AddVar("NodeAction",(int)_poObj->uiGetAction(),DEF_ACTION);
				}

			m_poFW->CloseSection();
			m_uiActionIdx++;
		}

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			WriteNodeWithSprite((CFESkelAnimSprite*)_poObj);
		}

		/// 
        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			WriteNodeWithSprite((CFESkelAnimMesh*)_poObj);
		}

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
			if ((!m_b1LevelBones) || (!m_bBoneVisited))
			{
				m_bBoneVisited = true;

				for (uint i=0;i<_poObj->uiNumChildren();i++)
					if (_poObj->poGetChild(i) != NULL)
						_poObj->poGetChild(i)->Accept(this);
			}
        }
};
//---------------------------------------------------------------------------
bool bSaveSkeletonAction(const CFEString& _sFilename,CSkeleton* _poSkeleton,bool _bAnimPivot)
{
	// make backup of file
	if (bFileExists(_sFilename))	
		MakeBackupOfFile( wxString(_sFilename.szString()) );

	bool bLoop = globals.GUI->cb_Loop->GetValue();
    ActionsPresaveProcess(_poSkeleton, _bAnimPivot);

        CFEString sAction = CFEStringUtils::sRemoveExtension( CFEStringUtils::sGetFilename( _sFilename ) );

        CLibConfigFileWriter oFW;
        if (! oFW.bCreate(std::string( _sFilename.szString()) ))
        {
			return(false);
        }
		
		uint uiNumNodeActions = 0;
		CFESkelAnim* poAnim = CFESkelAnimInstMgr::I()->poGetAnim(_poSkeleton->m_hInst);
		CFESkelAnimAction* poAction = _poSkeleton->m_poAnim->poGetAnimActionSet()->poGetAction(1);

        oFW.OpenSection("SkelAction");

            oFW.AddVar("Name",sAction.szString(),"");
            oFW.AddVar("PlayMode",bLoop?"loop":"oneshot",DEF_WRAP);

            for (uint i=0;i<poAction->uiNumNodeActions();i++)
            {
				CFESkelAnimNodeAction* poNodeAction = poAction->poGetNodeAction(i);

				bool bPos = (poNodeAction->m_oPosFunc.uiGetNumKeyFrames() > 1)
							||
							(
								(poNodeAction->m_oPosFunc.uiGetNumKeyFrames() == 1) 
								&& 
								(
									(poNodeAction->m_oPosFunc.poGetKeyFrameData(0)->m_oKey.x != DEF_POS_X) 
									|| 
									(poNodeAction->m_oPosFunc.poGetKeyFrameData(0)->m_oKey.y != DEF_POS_Y)
								)
							);

				bool bAngle = 
							(poNodeAction->m_rAngleFunc.uiGetNumKeyFrames() > 1) 
							||
							(
								(poNodeAction->m_rAngleFunc.uiGetNumKeyFrames() == 1)
								&& 
								(poNodeAction->m_rAngleFunc.poGetKeyFrameData(0)->m_oKey != DEF_ANGLE)
							);

				// Is there any animation info???
				if (bPos || bAngle)
				{
					oFW.OpenSection("NodeAction",uiNumNodeActions);

						oFW.AddVar("NodeName", poAnim->m_oNodeTab[ poNodeAction->uiGetNodeIdx() ]->sGetName().szString(),"");

						// position
						if (bPos)
						{
							oFW.OpenSection("PosFunc");

								WriteVectKeyFrames(poNodeAction->m_oPosFunc,&oFW,bLoop?"Loop":"FinalValue");

							oFW.CloseSection();
						}

						// angles
						if (bAngle)
						{
							oFW.OpenSection("AngleFunc");

								WriteFloatKeyFrames(poNodeAction->m_rAngleFunc,&oFW,bLoop?"Loop":"FinalValue");

							oFW.CloseSection();
						}

					oFW.CloseSection();
					uiNumNodeActions++;
				}
				
            }

			// ---------------
			// write child node actions... in case some sprites need to setup an specific action ...
			// NOTE: These are not in the table of node actions
			// ---------------
			for (uint b=0;b<_poSkeleton->m_oBone.size();b++)
            {
				if (_poSkeleton->m_oBone[b].bEditable())
					uiNumNodeActions = CNodeActionWritter::Write(_poSkeleton->m_oBone[b].m_poBone,&oFW,uiNumNodeActions,true);
            }

			oFW.AddVar("NumNodeActions",uiNumNodeActions,0);

        oFW.CloseSection();
        oFW.Write();

    ActionsPostsaveProcess(_poSkeleton);
    return(true);
}
//---------------------------------------------------------------------------
#if 0
	// previous way of saving animations

            for (uint b=0;b<_poSkeleton->m_oBone.size();b++)
            {
            
                if (! _poSkeleton->m_oBone[b].bEditable()) continue;

					/*
					if (! _poSkeleton->m_oBone[b].m_poBone->bIsVisible())
						oFW.AddVar("Visible", false, DEF_VISIBILITY);
					*/
					/*
					if (! _poSkeleton->m_oBone[b].m_poBJNodeAction->bIsVisible())
						oFW.AddVar("Visible", false, DEF_VISIBILITY);
					else
					*/
					{
						

					}



                // ---------------
                // Bone actions
                // ---------------
            	uint uiBNodeIdx = _poSkeleton->m_oBone[b].m_poBoneAction->uiGetNodeIdx();

				if (uiBNodeIdx < poAnim->m_oNodeTab.size())
				{
					oFW.OpenSection("NodeAction",uiNumNodeActions);

						oFW.AddVar("NodeName", poAnim->m_oNodeTab[uiBNodeIdx]->sGetName().szString(),"");

						// position
						oFW.OpenSection("PosFunc");

							WriteVectKeyFrames(_poSkeleton->m_oBone[b].m_poBJointAction->m_oPosFunc,&oFW,bLoop?"Loop":"FinalValue");

						oFW.CloseSection();

						// angles
						oFW.OpenSection("AngleFunc");

							WriteFloatKeyFrames(_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc,&oFW,bLoop?"Loop":"FinalValue");

						oFW.CloseSection();

					oFW.CloseSection();
					uiNumNodeActions++;
				}

				if (_poSkeleton->m_oBone[b].m_poParentJoint != NULL)
				{
					uint uiPNodeIdx = _poSkeleton->m_oBone[b].m_poPJointAction->uiGetNodeIdx();
					if (uiPNodeIdx <poAnim->m_oNodeTab.size())
					{
						oFW.OpenSection("NodeAction",uiNumNodeActions);

							oFW.AddVar("NodeName", poAnim->m_oNodeTab[uiPNodeIdx]->sGetName().szString(),"");

							// position
							oFW.OpenSection("PosFunc");

								WriteFloatKeyFrames(_poSkeleton->m_oBone[b].m_poPJointAction->m_rAngleFunc,&oFW,bLoop?"Loop":"FinalValue");

							oFW.CloseSection();

						oFW.CloseSection();
						uiNumNodeActions++;
					}
				}

				// ---------------
				// write child node actions... in case some sprites need to setup an specific action ...
				// ---------------
				uiNumNodeActions = CNodeActionWritter::Write(_poSkeleton->m_oBone[b].m_poBone,&oFW,uiNumNodeActions);
            }

            oFW.AddVar("NumNodeActions",uiNumNodeActions,(uint)0);

#endif