//-----------------------------------------------------------------------------
/*! \class Editor Utilities
 *  \brief 
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef TinyXMLUtilsH
#define TinyXMLUtilsH
// ----------------------------------------------------------------------------
class TiXmlNode;
// ----------------------------------------------------------------------------
void errorLog(char* str);
void infoLog(char* str);
void warningLog(char* str);

const char* readXmlString( TiXmlNode* parent, char* poNodeName, char* defaultValue, bool bCanBeEmpty = false);
float readXmlFloat( TiXmlNode* parent, char* poNodeName, float defaultValue, bool bCanBeEmpty = false, bool bShouldExists = true);
int readXmlInt( TiXmlNode* parent, char* poNodeName, int defaultValue, bool bCanBeEmpty = false, bool bShouldExists = true);
bool readXmlBool( TiXmlNode* parent, char* poNodeName, bool defaultValue, bool bShouldExists = true);
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
