#pragma warning(4996,"off")

#include <windows.h>
#include <direct.h>
#include <io.h>
#include <vector>
#include <string>
#include "CFileProcessor.h"
#define MAX_CHARS 1024
// ----------------------------------------------------------------------------
/// Default constructor for the class. Set's up the default options of the processor
// ----------------------------------------------------------------------------
CFileProcessor::CFileProcessor()
{
	m_iRequiredParams = 0;
	m_bRecursive = false;
}
// ----------------------------------------------------------------------------
void CFileProcessor::Error(int _Error,char* arg)
{
	switch (_Error)
	{	
		case 1:
		{
			printf( "ERROR: Invalid number of arguments.\n"
					"	Usage: %s\n",arg);
		}
		break;

		case 2:
		{
			printf("ERROR: Unable to open input file %s\n",arg);
		}
		break;

		case 3:
		{
			printf("ERROR: Unable to open output file %s\n",arg);
		}
		break;
	}

	exit(_Error);
}
// ----------------------------------------------------------------------------
/// Called when the processor enters a new directory.
// ----------------------------------------------------------------------------				
void CFileProcessor::EnterDir(const std::string& _sDirectory)
{
	
}
// ----------------------------------------------------------------------------
/// Called when the processor starts its job.
// ----------------------------------------------------------------------------
void CFileProcessor::StartProcess()
{
	
}
// ----------------------------------------------------------------------------
/// Called when the processor finishes its job.
// ----------------------------------------------------------------------------
void CFileProcessor::EndProcess()
{
	
}
// ----------------------------------------------------------------------------
/// Processes the given filename.
// ----------------------------------------------------------------------------
void CFileProcessor::Process(const std::string& _sFilename)
{

}
// ----------------------------------------------------------------------------
/// Processes the files of a directory.
// ----------------------------------------------------------------------------
void CFileProcessor::ProcessFiles_INT(const std::string& _sFindStr)
{
    // ------------------------------------
    // Parse all the files inside the current dir
    // ------------------------------------
    _finddata_t fdata;
    intptr_t ipt = _findfirst(_sFindStr.c_str(), &fdata);

    if (ipt != -1)
    {
        do{
			// if it's a file ...
            if (! (fdata.attrib & _A_SUBDIR))
            {
				std::string sFilename = std::string(fdata.name);
				Process(sFilename);
            }
        }while (_findnext(ipt,&fdata) != -1);

        _findclose(ipt);
    }

	// if not recursive stop
	if (! m_bRecursive) return;

    // ------------------------------------
    // Read subdirectories
    // ------------------------------------
    std::vector <std::string> oSubDirs;
    oSubDirs.clear();
    ipt = _findfirst("*.*", &fdata);

    if (ipt != -1)
    {
        do{
			// if it's a directory ...
            if (fdata.attrib & _A_SUBDIR)
            {
				std::string sFilename = std::string(fdata.name);

                // if ((sFilename != ".") && (sFilename != ".."))           
                if ( (sFilename != ".") && (sFilename != "..") )
                    oSubDirs.push_back(sFilename);
            }
        }while (_findnext(ipt,&fdata) != -1);

        _findclose(ipt);
    }

    // --------------------------------
    // For each subdirectory, recurse
    // --------------------------------
    for (int i=0;i<oSubDirs.size();i++)
    {
        _chdir(oSubDirs[i].c_str());
        EnterDir(oSubDirs[i]);
        ProcessFiles_INT(_sFindStr);
        _chdir("..");
    }
}
// ----------------------------------------------------------------------------
/// Processes the files of a directory.
// ----------------------------------------------------------------------------
void CFileProcessor::ProcessFiles(const std::string& _sFindStr)
{
	std::string sFindStr;

	if (_sFindStr=="")
	{
		if (! m_bRecursive)
			sFindStr = m_sInputFilename;
		else
			sFindStr = "*.*";
	}
	else
		sFindStr = _sFindStr;
	
	StartProcess();	
    
    ProcessFiles_INT(sFindStr);
    
    EndProcess();
}
// ----------------------------------------------------------------------------
/// Parses the command line and sets up options
// ----------------------------------------------------------------------------
void CFileProcessor::ParseCommandLine(int argc, char* argv[])
{
	/*
	for (int i=0;i<argc;i++)
		printf( "%s ",argv[i] );

	printf("\n\n");
	*/

	if ((argc-1) < m_iRequiredParams)
	{
		ShowUsage();
	}

	for (int i=1; i<argc; i++)
	{
		if (!stricmp(argv[i], "-r"))
			m_bRecursive = true;

   else if (!stricmp(argv[i], "-o"))
		{
			if ((i+1) < argc)
				m_sOutputFilename = std::string( argv[i+1] );

			i++;
		}
   else
		{
			// Takeout quotes (when starting directly using windows explorer features:
			// double click on an specific filetype, or using sendto feature)
			char szFilename[MAX_CHARS];

			if (argv[i][0] == '"')
			{
				strcpy(szFilename,argv[i]+1);
				int iLen = strlen(szFilename);

				if (szFilename[iLen-1] = '"')
					szFilename[iLen-1] = 0;
			}
			else
				strcpy( szFilename,argv[i] );

			m_sInputFilename = std::string(szFilename);

			// Get current working directory
			/*
			strcpy(gszWorkingDir,szFilename);
			ExtractPath(gszWorkingDir);
			// warning("Working Dir: %s",gszWorkingDir);

			// Get only filename
			gbWorkingDir = strlen(gszWorkingDir);
			if (gbWorkingDir)
				strcpy(gszInputFile,szFilename + strlen(gszWorkingDir) + 1);
			else
				strcpy(gszInputFile,szFilename); 
			// warning("InputFile: %s",gszInputFile);

			// Take out extension from filename
			TakeOutExtension(gszInputFile);
			// warning("Without ext: %s",gszInputFile);
			*/
		}
	}	
}
// ----------------------------------------------------------------------------
///	Shows the usage message of the file processor
// ----------------------------------------------------------------------------
void CFileProcessor::ShowUsage()
{
	printf("nothing to say...\n");
	exit(0);
}
// ----------------------------------------------------------------------------