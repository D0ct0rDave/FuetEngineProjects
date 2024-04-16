// Autoversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "autoversioning/avHeader.h"
#include "autoversioning/avConfig.h"

#include <wx/datetime.h>
#include <wx/File.h>
// #include <wx/msgdlg.h>
// ----------------------------------------------------------------------------
#define cbC2U(xxx) wxString(xxx)
#define cbU2C(xxx) xxx
// ----------------------------------------------------------------------------
avConfig            goConfig;
avHeader            goHeader;
avVersionState      goVersionState;
wxString            m_versionHeaderPath;
wxString            m_versionNameSpace;
// ----------------------------------------------------------------------------
avVersionState& GetVersionState()
{
    return(goVersionState);
}
// ----------------------------------------------------------------------------
avConfig& GetConfig()
{
    return(goConfig);
}
// ----------------------------------------------------------------------------
void UpdateVersionHeader()
{
    // DMC m_timerStatus->Stop();

    wxString headerOutput = _T("");
    headerOutput << _T("#ifndef ") << m_versionNameSpace << _T("H\n");
    headerOutput << _T("#define ") << m_versionNameSpace << _T("H\n");
    headerOutput << _T("\n");

    if(cbC2U(GetConfig().Settings.Language.c_str()) == _T("C++"))
    {
        headerOutput << _T("namespace ") << m_versionNameSpace << _T("{\n");
        headerOutput << _T("\t") << _T("\n");
    }

    if(GetConfig().Settings.DateDeclarations)
    {
        wxDateTime actualDate = wxDateTime::Now();
        headerOutput << _T("\t") << _T("//Date Version Types") << _T("\n");
        headerOutput << _T("\t") << _T("static const char DATE[] = ") << actualDate.Format(_T("\"%d\"")) << _T(";\n");
        headerOutput << _T("\t") << _T("static const char MONTH[] = ") << actualDate.Format(_T("\"%m\"")) << _T(";\n");
        headerOutput << _T("\t") << _T("static const char YEAR[] = ") << actualDate.Format(_T("\"%Y\"")) << _T(";\n");
        long ubuntuYearNumber = 0;
        actualDate.Format(_T("%y")).ToLong(&ubuntuYearNumber);
        wxString ubuntuYear;
        ubuntuYear.Printf(_T("%d"),ubuntuYearNumber);
        headerOutput << _T("\t") << _T("static const double UBUNTU_VERSION_STYLE = ") << ubuntuYear << actualDate.Format(_T(".%m")) << _T(";\n");
        headerOutput << _T("\t") << _T("\n");
    }

    headerOutput << _T("\t") << _T("//Software Status") << _T("\n");
    headerOutput << _T("\t") << _T("static const char STATUS[] = \"") << cbC2U(GetVersionState().Status.SoftwareStatus.c_str()) << _T("\";\n");
    headerOutput << _T("\t") << _T("static const char STATUS_SHORT[] = \"") << cbC2U(GetVersionState().Status.Abbreviation.c_str()) << _T("\";\n");
    headerOutput << _T("\t") << _T("\n");

    wxString myPrintf;
    headerOutput << _T("\t") << _T("//Standard Version Type") << _T("\n");
    myPrintf.Printf(_T("%d"), GetVersionState().Values.Major);
    headerOutput << _T("\t") << _T("static const long MAJOR = ") << myPrintf << _T(";\n");
    myPrintf.Printf(_T("%d"), GetVersionState().Values.Minor);
    headerOutput << _T("\t") << _T("static const long MINOR = ") << myPrintf << _T(";\n");
    myPrintf.Printf(_T("%d"), GetVersionState().Values.Build);
    headerOutput << _T("\t") << _T("static const long BUILD = ") << myPrintf << _T(";\n");
    myPrintf.Printf(_T("%d"), GetVersionState().Values.Revision);
    headerOutput << _T("\t") << _T("static const long REVISION = ") << myPrintf << _T(";\n");
    headerOutput << _T("\t") << _T("\n");

    headerOutput << _T("\t") << _T("//Miscellaneous Version Types") << _T("\n");
    myPrintf.Printf(_T("%d"), GetVersionState().Values.BuildCount);
    headerOutput << _T("\t") << _T("static const long BUILDS_COUNT = ") << myPrintf << _T(";\n");

    myPrintf.Printf(_T("%d,%d,%d,%d"), GetVersionState().Values.Major, GetVersionState().Values.Minor,
            GetVersionState().Values.Build, GetVersionState().Values.Revision);
    headerOutput << _T("\t") << _T("#define RC_FILEVERSION ") << myPrintf << _T("\n");

    myPrintf.Printf(_T("\"%d, %d, %d, %d\\0\""), GetVersionState().Values.Major, GetVersionState().Values.Minor,
            GetVersionState().Values.Build, GetVersionState().Values.Revision);
    headerOutput << _T("\t") << _T("#define RC_FILEVERSION_STRING ") << myPrintf << _T("\n");

    myPrintf.Printf(_T("\"%d.%d.%d.%d\""), GetVersionState().Values.Major, GetVersionState().Values.Minor,
            GetVersionState().Values.Build, GetVersionState().Values.Revision);
    headerOutput << _T("\t") << _T("static const char FULLVERSION_STRING[] = ") << myPrintf << _T(";\n");

	#if 0 
	// DMC
    if(GetConfig().Settings.Svn)
    {
        wxString revision,date;
        if (!QuerySvn(cbC2U(GetConfig().Settings.SvnDirectory.c_str()), revision, date))
            wxMessageBox(_("Svn configuration files not found.\nVerify the Autoversioning svn directory."),_("Error"),wxICON_ERROR);
        headerOutput << _T("\t") << _T("\n");
        headerOutput << _T("\t") << _T("//SVN Version") << _T("\n");
        headerOutput << _T("\t") << _T("static const char SVN_REVISION[] = ") << _T("\"") + revision + _T("\"")<< _T(";\n");
        headerOutput << _T("\t") << _T("static const char SVN_DATE[] = ") << _T("\"") + date + _T("\"")<< _T(";\n");
    }
    #endif

    headerOutput << _T("\t") << _T("\n");
    headerOutput << _T("\t") << _T("//These values are to keep track of your versioning state, don't modify them.") << _T("\n");
    myPrintf.Printf(_T("%d"), GetVersionState().BuildHistory);
    headerOutput << _T("\t") << _T("static const long BUILD_HISTORY = ") << myPrintf << _T(";\n");

    headerOutput << _T("\t") << _T("\n\n");

    if(cbC2U(GetConfig().Settings.Language.c_str()) == _T("C++"))
    {
        headerOutput << _T("}") << _T("\n");
    }

    headerOutput << _T("#endif //") << m_versionNameSpace << _T("H\n");

    // DMC: m_versionHeaderPath = FileNormalize(cbC2U(GetConfig().Settings.HeaderPath.c_str()),m_Project->GetBasePath());;
    wxFile versionHeaderFile(m_versionHeaderPath, wxFile::write);
    versionHeaderFile.Write(headerOutput);
    versionHeaderFile.Close();

    // DMC: m_timerStatus->Start(1000);
}
// ----------------------------------------------------------------------------
void CommitChanges()
{
    if (1) // DMC: m_Project && IsAttached() && m_IsCurrentProjectVersioned)
    {
        if (1) // DMC: m_Modified)
        {
            srand((unsigned)time(0));
            GetVersionState().Values.Revision += 1 + rand( ) % GetConfig().Scheme.RevisionRandMax;

            if ((GetConfig().Scheme.RevisionMax != 0) && (GetVersionState().Values.Revision > GetConfig().Scheme.RevisionMax))
            {
                GetVersionState().Values.Revision = 0;
            }

            if (GetConfig().Scheme.BuildMax == 0)
            {
                ++GetVersionState().Values.Build;
            }
            else if(GetVersionState().Values.Build >= GetConfig().Scheme.BuildMax)
            {
                GetVersionState().Values.Build = 0;
            }
            else
            {
                ++GetVersionState().Values.Build;
            }
            if(GetConfig().Settings.Autoincrement)
            {
                ++GetVersionState().BuildHistory;
                if(GetVersionState().BuildHistory >= GetConfig().Scheme.BuildTimesToIncrementMinor)
                {
                    GetVersionState().BuildHistory = 0;
                    ++GetVersionState().Values.Minor;
                }
                if (GetVersionState().Values.Minor > GetConfig().Scheme.MinorMax)
                {
                    GetVersionState().Values.Minor = 0;
                    ++GetVersionState().Values.Major;
                }
            }
            #if 0
            //DMC:
            if(GetConfig().ChangesLog.ShowChangesEditor)
            {
                GenerateChanges();
            }
            m_Project->SaveAllFiles();
            m_Modified = false;
            #endif
            UpdateVersionHeader();
        }
    }

}
// ----------------------------------------------------------------------------
// be activated and each has the compilerstarted/Finished ?????
void OnCompilerStarted(/*CodeBlocksEvent& event*/)
{
    if (1) // DMC: m_Project && IsAttached() && m_IsCurrentProjectVersioned)
    {
		if (1) // DMC: m_Modified)
		{
			const bool doAutoIncrement = GetConfig().Settings.DoAutoIncrement;
			const bool askToIncrement = GetConfig().Settings.AskToIncrement;

			if (doAutoIncrement && askToIncrement)
			{
				if (1) // DMC: wxMessageBox(_T("Do you want to increment the version?"),_T(""),wxYES_NO) == wxYES)
				{
					CommitChanges();
				}
			}
			else if(doAutoIncrement)
			{
				CommitChanges();
			}
		}
    }
} // end of OnCompilerStarted
// ----------------------------------------------------------------------------
void OnCompilerFinished(/*CodeBlocksEvent& event*/)
{
    if (1) // DMC: m_Project && IsAttached() && m_IsCurrentProjectVersioned)
	{
		++(GetVersionState().Values.BuildCount);
	}
} // end of OnCompilerFinished

// ----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{    
	avHeader        VersionHeader;

	if (argc > 1)
	    m_versionHeaderPath = _T(argv[1]);
	else
	    m_versionHeaderPath = _T("version.h");

    if (argc > 2)
        m_versionNameSpace = _T(argv[2]);
    else
        m_versionNameSpace = _T("AutoVersion");

    // ------------------------------------------------
    // Initial settings (default ones)
    // ------------------------------------------------
    goConfig.Settings.Autoincrement = 1;
    goConfig.Settings.DoAutoIncrement = 1;
    goConfig.Settings.AskToIncrement = 0;
    goConfig.Settings.DateDeclarations = 1;

    goConfig.Settings.HeaderPath = m_versionHeaderPath;
    goConfig.Settings.Language = _T("C++");    
    goConfig.Settings.Svn  = 0;
    goConfig.Settings.SvnDirectory = _T("");

    goConfig.Scheme.MinorMax = 10;
    goConfig.Scheme.BuildMax = 0;
    goConfig.Scheme.RevisionMax = 0;
    goConfig.Scheme.RevisionRandMax = 10;
    goConfig.Scheme.BuildTimesToIncrementMinor = 100;
    // ------------------------------------------------

    if(VersionHeader.LoadFile( m_versionHeaderPath ))
    {
        goVersionState.Values.Major = VersionHeader.GetValue(_T("MAJOR"));
        goVersionState.Values.Minor = VersionHeader.GetValue(_T("MINOR"));
        goVersionState.Values.Build = VersionHeader.GetValue(_T("BUILD"));
        goVersionState.Values.Revision = VersionHeader.GetValue(_T("REVISION"));
        goVersionState.Values.BuildCount = VersionHeader.GetValue(_T("BUILDS_COUNT"));
        goVersionState.Status.SoftwareStatus = cbU2C(VersionHeader.GetString(_T("STATUS")));
        goVersionState.Status.Abbreviation = cbU2C(VersionHeader.GetString(_T("STATUS_SHORT")));
        goVersionState.BuildHistory = VersionHeader.GetValue(_T("BUILD_HISTORY"));

		OnCompilerFinished();
        OnCompilerStarted();        
    }

	return 0;
}
// ----------------------------------------------------------------------------
