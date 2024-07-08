//-----------------------------------------------------------------------------
/*! \class TinyXML Utilities
 *  \brief 
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#include "TinyXMLUtils.h"
#include <tinyxml.h>
// ------------------------------------------------------------------------------------------------------
void errorLog(char* str)
{
	printf("ERROR %s\n",str);
	exit(1);
}

void infoLog(char* str)
{
    printf("INFO %s\n",str);
}

void warningLog(char* str)
{
    printf("WARNING %s\n",str);
}
// ------------------------------------------------------------------------------------------------------
const char* readXmlString( TiXmlNode* parent, char* poNodeName, char* defaultValue, bool bCanBeEmpty)
{
	TiXmlNode* poNode;
	if ( ( poNode = parent->FirstChild( poNodeName ) ) )
	{
		if ( poNode->FirstChild() )
		{
			return poNode->FirstChild()->Value();
		}
		else
		{
			if (! bCanBeEmpty)
			{
				// // warningLog( "[readXmlString] Using default value in " + poNodeName );
			}
			return defaultValue;
		}
	}
	else
	{
		// // warningLog( "[readXmlString] '" + poNodeName + "' poNode not found" );
		return defaultValue;
	}
}
// ------------------------------------------------------------------------------------------------------
float readXmlFloat( TiXmlNode* parent, char* poNodeName, float defaultValue, bool bCanBeEmpty, bool bShouldExists)
{
	TiXmlNode* poNode;
	if ( ( poNode = parent->FirstChild( poNodeName ) ) )
	{	
		if ( poNode->FirstChild() )
		{
			//float res = atof( poNode->FirstChild()->Value() );
			const unsigned int BUFSIZE = 1024;
			char szBuf[BUFSIZE];
			strcpy_s(szBuf,BUFSIZE,poNode->FirstChild()->Value() );
			szBuf[BUFSIZE-1] = 0;// just in case
		
			// avoid localization problems			
			char* szChar = szBuf;
			while (*szChar)
			{
				if (*szChar == ',')
					*szChar = '.';

				szChar++;					
			}

			float res = atof(szBuf);
			return res;
		}
		else
		{
			if (!bCanBeEmpty)
			{
				// warningLog( "[readXmlFloat] Using default value in " + poNodeName );
			}
			return defaultValue;
		}
	}
	else
	{
		if ( bShouldExists )
		{
			// warningLog( "[readXmlFloat] '" + poNodeName + "' poNode not found" );
		}
		return defaultValue;
	}
}
// ------------------------------------------------------------------------------------------------------
int readXmlInt( TiXmlNode* parent, char* poNodeName, int defaultValue, bool bCanBeEmpty, bool bShouldExists)
{
	TiXmlNode* poNode;
	if ( ( poNode = parent->FirstChild( poNodeName ) ) )
	{
		if ( poNode->FirstChild() )
    	{
		    return atoi( poNode->FirstChild()->Value() );
		}
		else
		{
			if (!bCanBeEmpty)
			{
				// warningLog( "[readXmlInt] Using default value in " + poNodeName );
			}
			return defaultValue;
		}
	}
	else
	{
		if (bShouldExists)
		{
			// warningLog( "[readXmlInt] '" + poNodeName + "' poNode not found" );
		}
		return defaultValue;
	}
}
// ------------------------------------------------------------------------------------------------------
bool readXmlBool( TiXmlNode* parent, char* poNodeName, bool defaultValue, bool bShouldExists)
{
	TiXmlNode* poNode;
	if ( ( poNode = parent->FirstChild( poNodeName ) ) )
	{
		if ( poNode->FirstChild() )
		{
		    const char *value;
		    if ( (value = poNode->FirstChild()->Value() ) && (! strcmp(value,"true" )))	
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			// warningLog( "[readXmlBool] Using default value in " + poNodeName );
			return defaultValue;
		}
	}
	else
	{
		if (bShouldExists)
		{
			// warningLog( "[readXmlBool] '" + poNodeName + "' poNode not found" );
		}
		return defaultValue;
    }
}
// ----------------------------------------------------------------------------
