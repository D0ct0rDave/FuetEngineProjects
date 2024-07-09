
//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CActorEditorGUIFileOps.h"
#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
#include "CAVIFile.h"
//---------------------------------------------------------------------------
bool bLoadFrameSequence(const CFEString& _sFilename)
{
    if (_sFilename == "") return(false);

	/// Is an avi file ?
    if (_sFilename.LowerCase().iPos(".avi") != -1)
    {
		/// Try opening it...
		if ( CAVIFile::bOpen(_sFilename.szString()) )
		{
			// Delete possible existant frames.
			globals.m_oFrames.erase(globals.m_oFrames.begin(),globals.m_oFrames.end());
			uint uiSize = CAVIFile::uiGetWidth()*CAVIFile::uiGetHeight()*4;

			for (uint i=0;i<CAVIFile::uiNumFrames();i++)
			{
				CAVIFile::GotoFrame(i);
				
				void* pData = malloc(uiSize);
				memcpy(pData,CAVIFile::pGetFrame(),uiSize);

				CFEString sMemFile;
				sMemFile.Format("MEM://0x%x/%d/%d",pData,CAVIFile::uiGetWidth(),CAVIFile::uiGetHeight());

				FEHandler hSpr = CFEMaterialMgr::I()->hLoad( sMemFile );
				if (hSpr != NULL)
				{
					globals.m_oFrames.push_back(hSpr);
				}
			}

			return(true);
		}
	}
	/// Is a lst file ?
    else if (_sFilename.LowerCase().iPos(".lst") != -1)
    {
		/// Try opening it...
		FILE* fd = fopen(_sFilename.szString(),"rb");
		if (fd != NULL)
		{
			// Delete possible existant frames.
			globals.m_oFrames.erase(globals.m_oFrames.begin(),globals.m_oFrames.end());

			char szOldDir[1024];
			GetCurrentDirectory(1024,szOldDir);
			
			// Get current directory from filename
			CFEString sCurDir = CFEStringUtils::sGetPath(_sFilename);
			SetCurrentDirectory(sCurDir.szString());

			char szFilename[1024];
			while (! feof(fd))
			{
				if (fscanf(fd,"%s",szFilename) == 1)
				{
					FEHandler hSpr = CFEMaterialMgr::I()->hLoad( CFEStringUtils::sRemoveExtension(szFilename) );
					globals.m_oFrames.push_back(hSpr);
				}
			}

			globals.m_sFrameFile = CFEString( _sFilename.szString() );

			SetCurrentDirectory(szOldDir);
			return(true);
		}
    }
    else
    {
		CFEString sFilename = CFEStringUtils::sRemoveExtension(_sFilename);

		// load just one frame...
		FEHandler hSpr = CFEMaterialMgr::I()->hLoad( sFilename);

		if (hSpr != NULL)
		{
			globals.m_oFrames.erase(globals.m_oFrames.begin(),globals.m_oFrames.end());
			globals.m_oFrames.push_back(hSpr);
			return(true);
		}
		else
		{
			return(false);
		}
	}

	return(false);
}
//---------------------------------------------------------------------------
