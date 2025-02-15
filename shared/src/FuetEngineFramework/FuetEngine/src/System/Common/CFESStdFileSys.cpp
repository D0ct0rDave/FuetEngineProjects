// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (FILESYS == USE_STDFILESYS)
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include "../CFESystem.h"
#include "FEBasicTypes.h"
#include "FEEnums.h"

// ----------------------------------------------------------------------------
/// Opens a given file in the given access mode and retrieves its file handler.
// ----------------------------------------------------------------------------
FEHandler CFESystem::File::hOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode)
{
	CFESystem::Log::Print("Loading %s ",_sFilename.szString());
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

	CFEString sFilename = _sFilename;
	if (CFESystem::Local::bIsAutoLocEnabled())
	{
		FILE* fd = NULL;
	
		// first try the non-localized or common version of the file.
		CFEString sLocalFilename = CFESystem::Local::sGetLocalID(LID_COMMON) + "/" + _sFilename;
		(FEHandler)fopen_s(&fd,sLocalFilename.szString(),szOpenMode);
		if (fd != NULL)
		{
			CFESystem::Log::Print("Ok\n");
			return(fd);
		}

		// then try localized version of the file.
		sLocalFilename = CFESystem::Local::sGetLocalID() + "/" + _sFilename;
		(FEHandler)fopen_s(&fd,sLocalFilename.szString(),szOpenMode);
		if (fd != NULL)
			CFESystem::Log::Print("Ok\n");
		else
			CFESystem::Log::Print("ERROR\n");
		
		return(fd);
	}
	else
	{
		FILE* fd = NULL;
		(FEHandler)fopen_s(&fd,_sFilename.szString(),szOpenMode);

		if (fd != NULL)
			CFESystem::Log::Print("Ok\n");
		else
			CFESystem::Log::Print("ERROR\n");

		return(fd);
	}
}
// ----------------------------------------------------------------------------
/// Closes the file associated to the given handler.
// ----------------------------------------------------------------------------
void CFESystem::File::Close(FEHandler _hFile)
{
    if (_hFile) fclose((FILE*)_hFile);
}
// ----------------------------------------------------------------------------
/// Reads an amount data from the file storing at the given buffer.
// ----------------------------------------------------------------------------
uint CFESystem::File::uiRead(FEHandler _hFile,void* _pData, uint _uiSize)
{
    return( fread(_pData,1,_uiSize,(FILE*)_hFile) );
}
// ----------------------------------------------------------------------------
/// Writes the amount of data from the given buffer to the file.
// ----------------------------------------------------------------------------
uint CFESystem::File::uiWrite(FEHandler _hFile,void* _pData, uint _uiSize)
{
    return(fwrite(_pData,1,_uiSize,(FILE*)_hFile));
}
// ----------------------------------------------------------------------------
/// Sets the read/write position of the file.
// ----------------------------------------------------------------------------
int CFESystem::File::iSeek(FEHandler _hFile,int _iOffset, EFEFileSeekMode _eMode)
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
uint CFESystem::File::uiPos(FEHandler _hFile)
{
    return(ftell((FILE*)_hFile));
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
