// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (FILESYS == USE_STDFILESYS)
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include "../CFECore.h"
#include "FEBasicTypes.h"
#include "FEEnums.h"

// ----------------------------------------------------------------------------
/// Opens a given file in the given access mode and retrieves its file handler.
// ----------------------------------------------------------------------------
FEHandler CFECore::File::hOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode)
{
	if (_sFilename == "")	
	{	
		CFECore::Log::Print("Unable to open file with NULL filename.\n");
		return(NULL);
	}

    char* szOpenMode = NULL;
    switch (_eMode)
    {
        case FOM_READ:
        szOpenMode = "rb";
        break;

        case FOM_WRITE:
        szOpenMode = "wb";
        break;

        default:
        return(false);
    }

	CFECore::Log::Print("Loading %s: ",_sFilename.szString());

	if (CFECore::Local::bIsAutoLocEnabled())
	{
		FILE* fd = NULL;
		CFEString sLocalFilename;

		// then try localized version of the file.
		sLocalFilename = CFECore::Local::sGetLocalID() + CFEString("/") + _sFilename;

        #ifndef __GNUC__
		fopen_s(&fd,sLocalFilename.szString(),szOpenMode);
		#else
		fd = fopen(sLocalFilename.szString(),szOpenMode);
		#endif
		if (fd != NULL)
		{
			CFECore::Log::Print("Ok (%s)\n",CFECore::Local::sGetLocalID().szString());
			return(fd);
		}

		// first try the non-localized or common version of the file.
		sLocalFilename = CFECore::Local::sGetLocalID(LID_COMMON) + CFEString("/") + _sFilename;

		#ifndef __GNUC__
		fopen_s(&fd,sLocalFilename.szString(),szOpenMode);
		#else
		fd = fopen(sLocalFilename.szString(),szOpenMode);
		#endif

		CFECore::Log::Print("%s (%s)\n",(fd != NULL)?"Ok":"Not found",CFECore::Local::sGetLocalID(LID_COMMON).szString());
		return(fd);
	}
	else
	{
		FILE* fd = NULL;

		#ifndef __GNUC__
		fopen_s(&fd,_sFilename.szString(),szOpenMode);
		#else
		fd = fopen(_sFilename.szString(),szOpenMode);
		#endif
		
		if (fd != NULL)
			CFECore::Log::Print("Ok\n");
		else
			CFECore::Log::Print("Not found\n");

		return(fd);
	}
}
// ----------------------------------------------------------------------------
/// Closes the file associated to the given handler.
// ----------------------------------------------------------------------------
void CFECore::File::Close(FEHandler _hFile)
{
    if (_hFile) fclose((FILE*)_hFile);
}
// ----------------------------------------------------------------------------
/// Reads an amount data from the file storing at the given buffer.
// ----------------------------------------------------------------------------
uint CFECore::File::uiRead(FEHandler _hFile,void* _pData, uint _uiSize)
{
    return( fread(_pData,1,_uiSize,(FILE*)_hFile) );
}
// ----------------------------------------------------------------------------
/// Writes the amount of data from the given buffer to the file.
// ----------------------------------------------------------------------------
uint CFECore::File::uiWrite(FEHandler _hFile,void* _pData, uint _uiSize)
{
    return(fwrite(_pData,1,_uiSize,(FILE*)_hFile));
}
// ----------------------------------------------------------------------------
/// Sets the read/write position of the file.
// ----------------------------------------------------------------------------
int CFECore::File::iSeek(FEHandler _hFile,int _iOffset, EFEFileSeekMode _eMode)
{
    switch (_eMode)
    {
        case FSM_SET:    return(fseek((FILE*)_hFile,_iOffset,SEEK_SET));
        case FSM_CUR:    return(fseek((FILE*)_hFile,_iOffset,SEEK_CUR));
        case FSM_END:    return(fseek((FILE*)_hFile,_iOffset,SEEK_END));
    }

    return(0);
}
// ----------------------------------------------------------------------------
/// Retrieves the current file read/write position.
// ----------------------------------------------------------------------------
uint CFECore::File::uiPos(FEHandler _hFile)
{
    return(ftell((FILE*)_hFile));
}
// ----------------------------------------------------------------------------
/// Retrieves the total file length
// ----------------------------------------------------------------------------
uint CFECore::File::uiLen(FEHandler _hFile)
{
	uint uiPosition = uiPos(_hFile);
	iSeek(_hFile,0,FSM_END);
	uint uiSize = uiPos(_hFile);
	iSeek(_hFile,uiPosition,FSM_SET);

    return(uiSize);
}
// ----------------------------------------------------------------------------
/// Reads the contents of a file and returns it as a memory buffer.
// ----------------------------------------------------------------------------
FEPointer CFECore::File::pReadFile(const CFEString& _sFilename,uint* _puiSize)
{
	FEHandler hFile = hOpen(_sFilename, FOM_READ);
	if (hFile != NULL)
	{
		uint uiSize = uiLen(hFile);
		FEPointer pBuffer = CFECore::Mem::pAlloc(uiSize);
		uiRead(hFile,pBuffer,uiSize);
		Close(hFile);

		if (_puiSize != NULL) *_puiSize = uiSize;
		return(pBuffer);
	}

	return(NULL);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
