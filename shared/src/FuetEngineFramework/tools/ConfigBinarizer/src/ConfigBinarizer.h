//-----------------------------------------------------------------------------
/*! \class CConfigFileWriter
 *  \brief Generic class to write configuration files.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 1999-2009
 *  \par Copyright (c) 2016 David Márquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef CConfigBinarizerH
#define CConfigBinarizerH
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
// Forward Declarations
class CConfigFileWriter;
//-----------------------------------------------------------------------------
class CConfigBinarizer
{
	public:

		std::string sBinarize(char** _szStream,CConfigFileWriter* _poFWriter);

		void Binarize(CConfigFileWriter* _poFWriter);

		void ParseCommandLine(int argc, char* argv[]);
	
	protected:
		std::string		m_sInputFilename;
		std::string		m_sOutputFilename;
};
#endif
//-----------------------------------------------------------------------------
