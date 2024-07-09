//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CSKEFileOpsH
#define CSKEFileOpsH
//---------------------------------------------------------------------------
#include <FuetEngine.h>
//---------------------------------------------------------------------------
class CSkeleton;
class CConfigFileWriter;
//---------------------------------------------------------------------------
class CSkeletonNodeWritter : public CFESkelAnimNodeVisitor
{
	protected:

		CConfigFileWriter*	m_poFW;
		CFEString			m_sWorkingDir;
		bool				m_bDescentBones;

	public:

		CSkeletonNodeWritter();

		static void Write(CFESkelAnimNode* _poNode,CConfigFileWriter* _poFW,const CFEString& _sWorkingDir,bool _bDescentBones = true);

		void WriteCommonProperties(CFESkelAnimNode* _poNode);

		/// 
        virtual void Visit(CFESkelAnimGroup* _poObj);

		template <class T>
		inline void WriteNodeWithSprite(T* _poObj)
		{
			WriteCommonProperties(_poObj);

			CFEString sExternalFile = CFESpriteMgr::I()->sGetResourceName( CFESpriteInstMgr::I()->poGetSprite(_poObj->hGetSprite()));
			uint uiAction = _poObj->uiGetAction();

			CFEString sFilename = sRelativizePath( sExternalFile, m_sWorkingDir);
			CFEString sSprite = CFEStringUtils::sRemoveExtension( CFEStringUtils::sNormalizePath(sFilename) );

			m_poFW->AddVar("Sprite",sSprite.szString(),"");
			m_poFW->AddVar("SpriteAction",(int)uiAction,DEF_ACTION);
		}

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj);
        
        /// 
        virtual void Visit(CFESkelAnimMesh* _poObj);

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj);
};
//---------------------------------------------------------------------------

void SaveSkeleton(const CFEString& _sFilename,CSkeleton* _poSkeleton);
void LoadSkeleton(const CFEString& _sFilename,CSkeleton* _poSkeleton);

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------