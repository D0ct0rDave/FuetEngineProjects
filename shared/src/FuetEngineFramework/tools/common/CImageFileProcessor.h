//-----------------------------------------------------------------------------
/*! \class CImageFileProcessor
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef CImageFileProcessorH
#define CImageFileProcessorH
//-----------------------------------------------------------------------------
#include "CFileProcessor.h"
#include "FreeImage.h"
#include "FreeImagePlus.h"
// ----------------------------------------------------------------------------
class CImageFileProcessor : public CFileProcessor
{
	public:

		virtual void Process(const std::string& _sFilename);
		
		virtual void Process(const fipImage& oImg);

		CImageFileProcessor()
		{
			m_iRequiredParams = 1;
		}
	
		std::string sGetProcessedFilename()
		{
			return( m_sProcessedFilename );
		}
	
	protected:

		std::string	m_sProcessedFilename;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
