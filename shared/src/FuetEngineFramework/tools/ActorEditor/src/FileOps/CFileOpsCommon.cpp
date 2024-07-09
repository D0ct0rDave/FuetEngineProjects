//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#include "CFileOpsCommon.h"
#include "edutils.h"
#include "CLibConfigFileWriter.h"
#include "CNULLSpriteInst.h"
//---------------------------------------------------------------------------
class CSkeletonNodeCloner : public CFESkelAnimNodeVisitor
{
	protected:

		CFESkelAnimNode*	m_poLoadedSrc;

	public:

		CSkeletonNodeCloner() : m_poLoadedSrc(NULL) {};

		static void Clone(CFESkelAnimNode* _poDstNode,CFESkelAnimNode* _poLoadedSrc)
		{
			CSkeletonNodeCloner	oObj;
			oObj.m_poLoadedSrc		= _poLoadedSrc;

			_poDstNode->Accept(&oObj);
		}

		void CloneCommonProperties(CFESkelAnimNode* _poSrcNode,CFESkelAnimNode* _poDstNode)
		{
			_poDstNode->SetDepth( _poSrcNode->rGetDepth() );			
			_poDstNode->SetAngle( _poSrcNode->rGetAngle() );
			_poDstNode->Show    ( _poSrcNode->bIsVisible() );
			_poDstNode->SetPos  ( _poSrcNode->oGetPos() );
			
			#ifdef USE_SKELANIM_SCALE_COLOR
			_poDstNode->SetColor( _poSrcNode->oGetColor() );
			_poDstNode->SetScale( _poSrcNode->oGetScale() );
			#endif
		}

		/// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			CFESkelAnimGroup* poSrcNode = (CFESkelAnimGroup*)CFESkelAnimNodeLocator::poLocateNode( m_poLoadedSrc, _poObj->sGetName() );
			if (poSrcNode != NULL)
				CloneCommonProperties(poSrcNode,_poObj);
			
			// Try children
			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

        /// 
		template <class T>
		inline void CloneNodeWithSprite(T* _poObj)
		{
			CFESpriteInstMgr::I()->ReleaseInstance( _poObj->hGetSprite() );
			_poObj->SetSprite(NULL);

			T* poSrcNode = (T*)CFESkelAnimNodeLocator::poLocateNode( m_poLoadedSrc, _poObj->sGetName() );
			if (poSrcNode != NULL)
			{
				CloneCommonProperties(poSrcNode,_poObj);

				FEHandler hSpriteInst = NULL;
				switch (CFESkelAnimNodeTypeGetter::eGetType(poSrcNode))
				{
					case SANT_SPRITE_MODEL:
					{
						hSpriteInst = CFESpriteInstMgr::I()->hGetInstance( ((CFESkelAnimSpriteModel*)poSrcNode)->sGetSprite() );
						// if (hSpriteInst == NULL) hSpriteInst = CNULLSpriteInst::I()->hGetInstance( ((CFESkelAnimSpriteModel*)poSrcNode)->sGetSprite() );
					}
					break;

					case SANT_MESH_MODEL:
					{
						hSpriteInst = CFESpriteInstMgr::I()->hGetInstance( ((CFESkelAnimMeshModel*)poSrcNode)->sGetSprite() );
						// if (hSpriteInst == NULL) hSpriteInst = CNULLSpriteInst::I()->hGetInstance( ((CFESkelAnimMeshModel*)poSrcNode)->sGetSprite() );
					}
					break;

					case SANT_SPRITE:
					{
						hSpriteInst  = (CFESkelAnimSprite*)poSrcNode->hGetSprite();
					}
					break;

					case SANT_MESH:
					{
						hSpriteInst  = (CFESkelAnimMesh*)poSrcNode->hGetSprite();
					}
					break;
				}

				if (hSpriteInst != NULL)
				{
					CFESpriteInstMgr::I()->Enable(hSpriteInst);
					_poObj->SetSprite( hSpriteInst );
					_poObj->SetAction( 0 );
				}
				else
				{
					_poObj->SetSprite( NULL );
				}
			}		
		}

        /// 
		virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			CloneNodeWithSprite((CFESkelAnimSprite*)_poObj);
		}
      
		/// 
        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			CloneNodeWithSprite((CFESkelAnimMesh*)_poObj);
		}
		
		/*
        /// 
		virtual void Visit(CFESkelAnimSpriteModel* _poObj)
        {
			CloneNodeWithSprite((CFESkelAnimSpriteModel*)_poObj);
		}

		/// 
        virtual void Visit(CFESkelAnimMeshModel* _poObj)
        {
			ClonelNodeWithSprite((CFESkelAnimMeshModel*)_poObj);
		}
		*/

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
			CFESkelAnimBone* poSrcNode = (CFESkelAnimBone*)CFESkelAnimNodeLocator::poLocateNode( m_poLoadedSrc, _poObj->sGetName() );

			if (poSrcNode != NULL)
			{
				_poObj->SetBonePos( poSrcNode->oGetBonePos());
				CloneCommonProperties(poSrcNode,_poObj);
			}

			/// Try child
			for (uint i=0;i<_poObj->uiNumChildren();i++)
			{
				if (_poObj->poGetChild(i) != NULL)
					_poObj->poGetChild(i)->Accept(this);
			}
        }
};
//-----------------------------------------------------------------------------
void CloneNodes(CFESkelAnimNode* _poDstNode,CFESkelAnimNode* _poLoadedSrc)
{
	CSkeletonNodeCloner::Clone(_poDstNode,_poLoadedSrc);
}
//-----------------------------------------------------------------------------
void WriteFloatKeyFrames(CFEKFBFunc<float>& _oCoords,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode)
{
	_poFWriter->AddVar("WrapMode",_sWrapMode,DEF_WRAP);
    _poFWriter->AddVar("NumKeyFrames",_oCoords.uiGetNumKeyFrames(),(int)DEF_KEYFRAMES);

    for (uint uiKeyFrame=0;uiKeyFrame<_oCoords.uiGetNumKeyFrames();uiKeyFrame++)
    {
        CFETypedKeyFrame<float>* poKF = _oCoords.poGetKeyFrameData(uiKeyFrame);

        std::string sLF = CFEKFBFuncUtils::sGetLerpFuncName(poKF->eGetLerpFunc()).szString();
        FEReal rVal     = poKF->m_oKey;

        _poFWriter->OpenSection("KeyFrame",uiKeyFrame);

            _poFWriter->AddVar("Value", rVal,DEF_ANGLE);
            _poFWriter->AddVar("Time",poKF->rGetKeyTime(),DEF_TIME);
            _poFWriter->AddVar("LerpFunc", sLF,DEF_LERP);

        _poFWriter->CloseSection();
    };
}
//-----------------------------------------------------------------------------
void WriteVectKeyFrames(CFEKFBFunc<CFEVect2>& _oCoords,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode)
{
	_poFWriter->AddVar("WrapMode",_sWrapMode,DEF_WRAP);
    _poFWriter->AddVar("NumKeyFrames",_oCoords.uiGetNumKeyFrames(),(int)DEF_KEYFRAMES);

    for (uint uiKeyFrame=0;uiKeyFrame<_oCoords.uiGetNumKeyFrames();uiKeyFrame++)
    {
        CFETypedKeyFrame<CFEVect2>* poKF = _oCoords.poGetKeyFrameData(uiKeyFrame);

        std::string sLF = CFEKFBFuncUtils::sGetLerpFuncName(poKF->eGetLerpFunc()).szString();
        CFEVect2 oPos = poKF->m_oKey;

        _poFWriter->OpenSection("KeyFrame",uiKeyFrame);

            _poFWriter->AddVar("x", oPos.x,DEF_POS_X);
            _poFWriter->AddVar("y", oPos.y,DEF_POS_Y);
            _poFWriter->AddVar("Time",poKF->rGetKeyTime(),DEF_TIME);
            _poFWriter->AddVar("LerpFunc", sLF,DEF_LERP);

        _poFWriter->CloseSection();
    };
}
//---------------------------------------------------------------------------