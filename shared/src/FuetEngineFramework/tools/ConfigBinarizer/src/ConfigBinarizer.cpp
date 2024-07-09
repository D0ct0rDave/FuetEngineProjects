
// Standard Gen Includes

#include <windows.h>
#include <math.h>
#include <direct.h>

#include <vector>
#include <string>
#include <math.h>
#include "ConfigBinarizer.h"
#include "CLibConfigFileWriter.h"
#include "CBinConfigFileWriter.h"
#include "deelx.h"
#include "stdafx.h"
// ---------------------------------------------------------------------------
/// Reads a given file from disk and returns in the the form of a memory buffer
// ---------------------------------------------------------------------------
char* pucReadFile(const char* _szFilename)
{
	// FILE* fd = fopen(_szFilename,"rb");	
	FILE* fd = NULL;
	fopen_s(&fd,_szFilename,"rb");	

	if (fd != NULL)
	{
		// Retrieve the file size
		fseek(fd,0,SEEK_END);
		unsigned int uiLen = ftell(fd);
		fseek(fd,0,SEEK_SET);

		// Create a buffer to hold the complete file contents
		char* pBuffer = (char*)malloc(uiLen+1);
		fread(pBuffer,uiLen,1,fd);
		pBuffer[uiLen] = 0;

		// Close the file
		fclose(fd);

		// Return the file contents
		return((char*)pBuffer);
	}

	return(NULL);
}
// ----------------------------------------------------------------------------
/// Checks whether a given string is a number or not
// ----------------------------------------------------------------------------
bool bIsNumber(const std::string& _sString)
{
	CRegexpT<char> myNum("((0x|0X)[a-fA-F0-9]+|[0-9]+)");
	MatchResult result = myNum.Match( _sString.c_str() );
	return ( result.IsMatched() > 0 );
}
// ----------------------------------------------------------------------------
// Checks if the given char is a "word" character (including numbers and "_" character)
// ----------------------------------------------------------------------------
inline bool bIsWordChar(char c)
{
	return( 	((c >= 'a') && (c <= 'z'))
			  ||((c >= 'A') && (c <= 'Z'))
			  ||((c >= '0') && (c <= '9'))
			  ||(c=='_')
		);
}
// ----------------------------------------------------------------------------
/// Checks whether the given string is a full word or not.
// ----------------------------------------------------------------------------
bool bIsWord(const std::string & _sString)
{
	/*
	CRegexpT<char> myNum("\A\w*\z");
	MatchResult result = myNum.Match( _sString.szString() );
	return ( result.IsMatched() );
    */

	for (unsigned int i=0;i<_sString.size();i++)
		if (! bIsWordChar(_sString[i])) return(false);

	return(true);
}
// ----------------------------------------------------------------------------
/// Builds a std::string object from basic fields.
// ----------------------------------------------------------------------------
std::string sBuildToken(const char* _szString,unsigned int _uiStart,unsigned int _uiLen)
{
	char* szStr = (char*)malloc(_uiLen + 1);
	memcpy(szStr,_szString + _uiStart,_uiLen);
	szStr[_uiLen] = 0;
	std::string sStr = std::string(szStr);
	free(szStr);

	return (sStr);
}
// ----------------------------------------------------------------------------
/// Preprocess the given stream looking for expressions defined by the given RegExp
// ----------------------------------------------------------------------------
void PreprocessRegExp(char* _szRegExp,char* _szStream)
{
	// ------------------------------------------
	// Find regexp and take them out (substitute by spaces)
	// ------------------------------------------
	MatchResult result;
	CRegexpT<char> oRegExp(_szRegExp);
	result = oRegExp.Match( _szStream );

	while (result.IsMatched())
	{
		// fill with spaces
		unsigned int uiLen = result.GetEnd() - result.GetStart();
		char* pucChar = _szStream + result.GetStart();
		for (unsigned int i=0;i<uiLen;i++)
		{
			// in order to properly count the line numbers we need the new line characters
			// if ((pucChar[i]!='\r') && (pucChar[i] != '\n'))
				pucChar[i] = ' ';
		}

		// find next
		result = oRegExp.Match(_szStream, result.GetEnd());
	}
}

// ----------------------------------------------------------------------------
void SubstChar(char* _szStream,char _cChar)
{
	while (*_szStream)
	{
	    if (*_szStream == _cChar)
	        *_szStream = ' ';
        
        _szStream++;   
	};
}
// ---------------------------------------------------------------------------
/// Preprocess a stream of data replacing comments by space chars
// ---------------------------------------------------------------------------
void PreprocessComments(char* _szStream)
{
	// ------------------------------------------
    // Find multiline comments and take them out
    // ------------------------------------------
    PreprocessRegExp("/\\*([^*]|[\\r\\n]|(\\*+([^*/]|[\\r\\n])))*\\*+/",_szStream);

	// ------------------------------------------
	// Find single line comments and take them out
	// ------------------------------------------
	PreprocessRegExp("//.*",_szStream);
}
// ---------------------------------------------------------------------------
/// Preprocess a stream of data and takes out empty sections
// ---------------------------------------------------------------------------
bool bRemoveEmptySections(char** _szStream){
    // ------------------------------------------
	// Look if the stream starts with a section
	// ------------------------------------------
	std::string sID = "";
	bool bEmptySection = true;
	char* szSectionStart = *_szStream;

	while ( *(*_szStream) )
	{
	    char c = *(*_szStream);
	    (*_szStream)++;

	    switch (c)
	    {
	        case '}':
	        {
				return( bEmptySection );
	        }
	        break;
	        
	        case ';':
	        {
				szSectionStart = (*_szStream)+1;
	        };
	        break;

	        case '{':
	        {
				if (bRemoveEmptySections(_szStream))
				{
					while (*szSectionStart != '}')
					{
						*szSectionStart = ' ';
						szSectionStart++;
					};

					// remove '}'
					while (*szSectionStart != ';')
					{
						*szSectionStart = ' ';
						szSectionStart++;						
					};

					// remove ';'
					*szSectionStart = ' ';
					szSectionStart++;
				}
				else
				{
					bEmptySection = false;
				}
			}
	        break;

	        case '=':
	        {
				bEmptySection = false;
            }
	        break;
	    }
	}

    return(true);
}
// ----------------------------------------------------------------------------
std::string sStripWhitespaces(const std::string& _sStr)
{
    if (_sStr == "") return("");

    std::string sRes = "";
	MatchResult result;
	
	CRegexpT<char> oRegExp("[ ]*\([^ ]*\)");
	result = oRegExp.Match( _sStr.c_str() );

	while( result.IsMatched() )
	{
	    std::string sToken = sBuildToken(_sStr.c_str(),result.GetGroupStart(1),result.GetGroupEnd(1) - result.GetGroupStart(1));
	    
        sRes += sToken;

	    // find next
		result = oRegExp.Match(_sStr.c_str() + result.GetStart(), result.GetEnd());
    }

    return(sRes);
}
// ----------------------------------------------------------------------------
std::string sStripQuotes(const std::string& _sStr)
{
    if (_sStr == "") return("");

    std::string sRes = "";
	MatchResult result;
    // char * szRegExp = "[\"]\(.*\)[\"]";
    char * szRegExp = "\"(.*)\"";
	CRegexpT<char> oRegExp(szRegExp,SINGLELINE);	// para matchear toda la string como si fuera una sola linea aunque hayan \n's....
	result = oRegExp.Match( _sStr.c_str() );
	if( result.IsMatched() )
	    return( sBuildToken(_sStr.c_str(),result.GetGroupStart(1),result.GetGroupEnd(1) - result.GetGroupStart(1)) );
    else
        return(_sStr);
}
// ----------------------------------------------------------------------------
std::string CConfigBinarizer::sBinarize(char** _szStream,CConfigFileWriter* _poFWriter)
{
    // ------------------------------------------
	// Look if the stream starts with a section
	// ------------------------------------------
	std::string sID = "";
	bool bInString = false;

	while ( *(*_szStream) )
	{
	    char c = *(*_szStream);
	    (*_szStream)++;

        if (bInString)
        {
			sID = sID + c;

			if (c == '"')
			{
				bInString = false;
			}
        }
        else
        {
			switch (c)
			{
				case ':':
				{
					// new section
					if (sID.find("\"") == std::string::npos)
					{
						sID = sStripWhitespaces(sID);
						_poFWriter->OpenSection(sID);

							sBinarize(_szStream,_poFWriter);

						_poFWriter->CloseSection();
						sID="";
					}
					else
					{
						// We are parsing a string value ...
						sID = sID + c;
					}
				}
				break;

				case '}':
				{
				}	        
				break;

				case '{':
				sID="";
				break;

				case '\n':
				case '\t':
				case ' ':
				{
				};
				break;

				// close section or variable
				case ';':				
				{					
					return( sID );
				}
				break;

				// close section or variable
				case '"':
				{
					// we need also quotes to identitfy numbers inside a string from plain numbers
					sID = sID + c;
					bInString = true;
				}
				break;

				case '=':
				{					
					// new variable
					sID = sStripWhitespaces(sID);
					std::string sValue = sBinarize(_szStream,_poFWriter);

					// Guess variable type:
					float fValue = 0;
					int iValue = 0;

					// float
					if ( (sValue.find(".") != std::string::npos) && (sscanf(sValue.c_str(),"%f",&fValue)) )
					{
						_poFWriter->AddVar(sID,fValue,fValue+1);
					}
					// integer
			   else if (sscanf(sValue.c_str(),"%d",&iValue))
					{
						_poFWriter->AddVar(sID,iValue,iValue+1);
					}
					// FEBoolean true
			   else if (! stricmp(sValue.c_str(),"true"))
					{
						_poFWriter->AddVar(sID,bool(true),bool(false));
					}
					// FEBoolean false
			   else if (! stricmp(sValue.c_str(),"false"))
					{
						_poFWriter->AddVar(sID,bool(false),bool(true));
					}
					// string
				else {
						_poFWriter->AddVar(sID,sStripQuotes(sValue),sValue+"_");
					}

					sID="";
				}
				break;

				default:
				{
					sID = sID + c;
				}
				break;
			}
		}
	}

    return("");
}
// ----------------------------------------------------------------------------
void CConfigBinarizer::Binarize(CConfigFileWriter* _poFWriter)
{
    char* szConfig = pucReadFile( m_sInputFilename.c_str() );
    if (szConfig == NULL)
    {
        printf("Unable to read input file %s\n",m_sInputFilename.c_str());
        return;
    }
	
	// Already binarized file????
	if (strncmp(szConfig,"BCFG",4) == 0)
	{
		char szString[1024];
		sprintf(szString,"copy /y %s %s",m_sInputFilename.c_str(),m_sOutputFilename.c_str());
		system(szString);
		return;
	}
	
    // CConfigFileWriter* poFWriter = new CLibConfigFileWriter(); // for debug purposes
    if (! _poFWriter->bCreate(m_sOutputFilename))
    {
        printf("Unable to create output file %s\n",m_sOutputFilename.c_str());
        return;
    }

    PreprocessComments(szConfig);
    // take out \r \n \t
    SubstChar(szConfig,'\t');
    SubstChar(szConfig,'\r');
    // SubstChar(szConfig,'\n');

    char* szData = szConfig;
    // Really don't needed: bRemoveEmptySections(&szConfig);  
    FILE* fd = fopen("c:/caca.txt","wt");
    if (fd)
	{
		fwrite(szData,strlen(szData),1,fd);
		fclose(fd);
	}
	    
    sBinarize(&szData,_poFWriter);
    _poFWriter->Write();
}
// ----------------------------------------------------------------------------
void CConfigBinarizer::ParseCommandLine(int argc, char* argv[])
{	
	for (int i=1; i<argc; i++)
	{
        if (!stricmp(argv[i], "-o"))
		{
			if ((i+1) < argc)
				m_sOutputFilename = std::string( argv[i+1] );

			i++;
		}
		else
		{
			// Takeout quotes (when starting directly using windows explorer features:
			// double click on an specific filetype, or using sendto feature)
			char szFilename[1024];

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
		}
	}
}
// ----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	CConfigBinarizer oCB;

	oCB.ParseCommandLine(argc,argv);
	oCB.Binarize(new CBinConfigFileWriter() );

	return(0);
}
// ----------------------------------------------------------------------------
