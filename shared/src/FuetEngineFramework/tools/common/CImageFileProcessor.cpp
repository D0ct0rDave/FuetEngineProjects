// Standard Gen Includes
#include <windows.h>
#include "CImageFileProcessor.h"
// ----------------------------------------------------------------------------
void CImageFileProcessor::Process(const fipImage& oImg)
{
	for (int j=0;j<oImg.getHeight();j++)
		for (int i=0;i<oImg.getWidth();i++)
		{
			
		}	
}
// ----------------------------------------------------------------------------
void CImageFileProcessor::Process(const std::string& _sFilename)
{
	m_sProcessedFilename = _sFilename;

	fipImage oImg;
	if ( oImg.load(_sFilename.c_str()) )
	{	
		char szCanonicalName[1024];
		GetFullPathName(m_sProcessedFilename.c_str(),1024,szCanonicalName,NULL);
		m_sProcessedFilename = szCanonicalName;

		oImg.convertTo32Bits();
		Process(oImg);	
	}
}
// ----------------------------------------------------------------------------
