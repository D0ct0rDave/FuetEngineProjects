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
#if (FILESYS == USE_PHYSIXFSFILESYS)
// ----------------------------------------------------------------------------
#ifdef WIN32
#include <io.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "physfs.h"

#include "../CFECore.h"
#include "FEBasicTypes.h"
#include "FEEnums.h"
#include "types/CFEArray.h"
// ----------------------------------------------------------------------------
static void AddPatchFiles()
{
	#ifdef WIN32
	// ------------------------------------
    // Parse all the files inside the current dir
    // ------------------------------------
    _finddata_t fdata;
    intptr_t ipt = _findfirst("data??.fed", &fdata);

    if (ipt != -1)
    {
        do{
			// if it's a file ...
            if (! (fdata.attrib & _A_SUBDIR))
            {
				// PHYSFS_addToSearchPath(fdata.name, 1);
				PHYSFS_mount(fdata.name, NULL, 1);
            }
        }while (_findnext(ipt,&fdata) != -1);

        _findclose(ipt);
    }
    #else
    #endif
}
// ----------------------------------------------------------------------------
static FEBool gsbInitialized = false;
static void InitializePhysixFS()
{
	if (!PHYSFS_isInit())
	{
		if (!PHYSFS_init(NULL))
		{
			// error
		}
	}

//	PHYSFS_addToSearchPath(szDATAARCHIVE, 0);
	const char* szDATAARCHIVE = "data.fed";
	PHYSFS_mount(szDATAARCHIVE, NULL, 0);

	// add "patch" files
	AddPatchFiles();
	
	gsbInitialized = true;
}
// ----------------------------------------------------------------------------
/// Opens a given file in the given access mode and retrieves its file handler.
// ----------------------------------------------------------------------------
FEHandler CFECore::File::hOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode)
{
	if (!gsbInitialized) InitializePhysixFS();
	CFECore::Log::Print("Opening %s: ",_sFilename.szString());

	CFEString sFilename = _sFilename.LowerCase();
	if (CFECore::Local::bIsAutoLocEnabled())
	{
		PHYSFS_File* fd = NULL;
		CFEString sLocalFilename;

		// then try localized version of the file.
		sLocalFilename = CFECore::Local::sGetLocalID() + "/" + sFilename;
		
		if (_eMode == FOM_READ)
			fd = PHYSFS_openRead(sLocalFilename.szString());
		else
			fd = PHYSFS_openWrite(sLocalFilename.szString());

		if (fd != NULL)
		{
			CFECore::Log::Print("Ok (%s)\n",CFECore::Local::sGetLocalID().szString());
			return(fd);
		}

		// first try the non-localized or common version of the file.
		sLocalFilename = CFECore::Local::sGetLocalID(LID_COMMON) + "/" + sFilename;

		if (_eMode == FOM_READ)
			fd = PHYSFS_openRead(sLocalFilename.szString());
		else
			fd = PHYSFS_openWrite(sLocalFilename.szString());
			
		CFECore::Log::Print("%s (%s)\n",(fd != NULL)?"Ok":"Not found",CFECore::Local::sGetLocalID(LID_COMMON).szString());

		return(fd);
	}
	else
	{
		PHYSFS_File* fd = NULL;
		if (_eMode == FOM_READ)
			fd = PHYSFS_openRead(sFilename.szString());
		else
			fd = PHYSFS_openWrite(sFilename.szString());

		if (fd != NULL)
			CFECore::Log::Print("Ok\n");
		else
			CFECore::Log::Print("Not found\n");

		return(fd);
	}

	return(NULL);
}
// ----------------------------------------------------------------------------
/// Closes the file associated to the given handler.
// ----------------------------------------------------------------------------
void CFECore::File::Close(FEHandler _hFile)
{
    if (!gsbInitialized) InitializePhysixFS();
    if (_hFile) PHYSFS_close((PHYSFS_File*)_hFile);
}
// ----------------------------------------------------------------------------
/// Reads an amount data from the file storing at the given buffer.
// ----------------------------------------------------------------------------
uint CFECore::File::uiRead(FEHandler _hFile,void* _pData, uint _uiSize)
{
    if (!gsbInitialized) InitializePhysixFS();
    return( PHYSFS_read((PHYSFS_File*)_hFile,_pData,1,_uiSize) );
}
// ----------------------------------------------------------------------------
/// Writes the amount of data from the given buffer to the file.
// ----------------------------------------------------------------------------
uint CFECore::File::uiWrite(FEHandler _hFile,void* _pData, uint _uiSize)
{
    if (!gsbInitialized) InitializePhysixFS();
    return(PHYSFS_write((PHYSFS_File*)_hFile,_pData,1,_uiSize));
}
// ----------------------------------------------------------------------------
/// Sets the read/write position of the file.
// ----------------------------------------------------------------------------
int CFECore::File::iSeek(FEHandler _hFile,int _iOffset, EFEFileSeekMode _eMode)
{
    if (!gsbInitialized) InitializePhysixFS();
    switch (_eMode)
    {
        case FSM_SET:    return(PHYSFS_seek((PHYSFS_File*)_hFile,_iOffset) );
        case FSM_CUR:    return(PHYSFS_seek((PHYSFS_File*)_hFile,_iOffset - uiPos(_hFile)) );
        case FSM_END:    return(PHYSFS_seek((PHYSFS_File*)_hFile,uiLen(_hFile) - _iOffset) );
    }

    return(0);
}
// ----------------------------------------------------------------------------
/// Retrieves the current file read/write position.
// ----------------------------------------------------------------------------
uint CFECore::File::uiPos(FEHandler _hFile)
{
    if (!gsbInitialized) InitializePhysixFS();
    return(PHYSFS_tell((PHYSFS_File*)_hFile));
}
// ----------------------------------------------------------------------------
/// Retrieves the total file length
// ----------------------------------------------------------------------------     
uint CFECore::File::uiLen(FEHandler _hFile)
{
    if (!gsbInitialized) InitializePhysixFS();
	return( PHYSFS_fileLength((PHYSFS_File*)_hFile ) );
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
