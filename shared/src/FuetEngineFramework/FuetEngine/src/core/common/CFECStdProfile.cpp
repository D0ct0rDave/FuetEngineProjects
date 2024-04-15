// ----------------------------------------------------------------------------
/*! \class
 *  \brief Profile functionality based on standard filesystem files.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (PROFILE == USE_STDPROFILE)
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <math.h>

#include "../CFECore.h"
#include "../CFECoreGlobals.h"
#include "FEBasicTypes.h"
#include "FECStdMessages.h"
#include "support/misc/CFEStringUtils.h"
// ----------------------------------------------------------------------------
const uint 		MIN_SIZE_TO_SHOW_MESSAGE = 16*1024*1024;
const FEReal	TIME_TO_OPEN_MSGBOX		= _075r;
const FEReal	TIME_TO_READ_MSGBOX		= _3r;
// ----------------------------------------------------------------------------
FEReal	m_rTimeToSaveProfileData = 0.0f;
FEReal	m_rTimeToLoadProfileData = 0.0f;
// ----------------------------------------------------------------------------
const uint CRC_SIZE 		= sizeof(uint);
const uint PROFILEID_SIZE 	= 8;
const uint VERSION_SIZE 	= sizeof(uint);
const uint CRYPTOKEY_SIZE 	= 16;

const uint PROFILEID_OFFSET		= 0;
const uint ENCRIPT_DATA_OFFSET	= PROFILEID_SIZE + VERSION_SIZE;
const uint DATA_OFFSET			= PROFILEID_SIZE + VERSION_SIZE + CRC_SIZE ;

static unsigned char PROFILEID[PROFILEID_SIZE] = {'@','H','M','H','@','A','M','H'};
static unsigned char CRYPTOKEY[CRYPTOKEY_SIZE] = {'@','D','M','C','@','C','H','B',
												  0x5a,0xa5,0xa5,0x5a,0x3f,0xc1,0xd9,0x53};

typedef struct
{
	unsigned char	m_ucID[PROFILEID_SIZE];
	uint			m_uiVersion;
}TProfileHeader;
// ----------------------------------------------------------------------------
CFEString sGetProfileFilename(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename.LowerCase();
	
	char* szStr = (char*)sFilename.szString();
	for (uint i=0;i<sFilename.uiLen();i++)
	{
		if (szStr[i] == ':') szStr[i] = '.';
		if (szStr[i] == '/') szStr[i] = '-';
		if (szStr[i] == '\\') szStr[i] = '-';
	}

	return(sFilename+".sav");
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Profile functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
namespace CFECore { namespace Profile {
	
	FEBool bDumpSaveData(FILE* f,FEPointer _pData, uint _uiSize);
	void BuildSaveData(FEPointer _pBuffer,uint _uiBuffSize,FEPointer _pData, uint _uiSize);

	FEBool oGenericSave(FEPointer _pData, uint _uiSize);
	FEBool oGenericLoad(FEPointer _pData, uint _uiSize);

	// helper functions
	FEBool bCreateSaveData(FEPointer _pData, uint _uiSize,FEBool _bForceShowMessage);
	FEBool bCreateFileStructure(FEPointer _pData, uint _uiSize);
	FEBool bRecoverFileSystem(FEPointer _pData, uint _uiSize,FEBool _bForceShowMessage);
	
	void ShowMessage(const char* _szMessage,const char* _szMessageOptions);

	void OpenMessageBox(uint _uiSize,FETick _oIniTicks,const char* _szMessage,const char* _szMessageOptions,FEBool _bForceShowMessage);
	void WaitAndCloseMessageBox(uint _uiSize,FETick _oIniTicks,const char* _szMessage,const char* _szMessageOptions,FEBool _bForceShowMessage);
}}
// #define DEBUG_MESSAGES
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Profile functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFECore::Profile::RegisterApplication(const CFEString& _sApplicationName,uint _uiApplicationVersion)
{
	FECStdMessages::Init();

	FECglobals.m_sApplicationName		= _sApplicationName;
	FECglobals.m_uiApplicationVersion	= _uiApplicationVersion;
}
// ----------------------------------------------------------------------------
void CFECore::Profile::OpenMessageBox(uint _uiSize,FETick _oIniTicks,const char* _szMessage,const char* _szMessageOptions,FEBool _bForceShowMessage)
{
	if ((_uiSize < MIN_SIZE_TO_SHOW_MESSAGE) && (!_bForceShowMessage)) return;

	FEReal rTime = _0r;
	CFECore::TRC::OpenMessageBox(_szMessage,_szMessageOptions);

	// Open the message box
	do
	{
		CFECore::TRC::CheckStep();
		
		CFECore::TRC::iMessageBoxStep();
		FETick oCurTicks = CFECore::oGetEngineTicks();
		rTime = rGetTickTime(oCurTicks-_oIniTicks);
	}while (rTime<TIME_TO_OPEN_MSGBOX);
}
// ----------------------------------------------------------------------------
void CFECore::Profile::WaitAndCloseMessageBox(uint _uiSize,FETick _oIniTicks,const char* _szMessage,const char* _szMessageOptions,FEBool _bForceShowMessage)
{
	if ((_uiSize < MIN_SIZE_TO_SHOW_MESSAGE) && (!_bForceShowMessage)) return;

	FEReal rTime = _0r;

	// Wait enough time to let the user read the text or press some option ...
	int iRes = -1;
	do
	{
	
			CFECore::TRC::CheckStep();

			iRes = CFECore::TRC::iMessageBoxStep();
			FETick oCurTicks = CFECore::oGetEngineTicks();
			rTime = rGetTickTime(oCurTicks-_oIniTicks);

	}while ( 
				// no hay opciones y el tiempo de lectura es inferior al límite
			  ((_szMessageOptions == NULL) && (rTime < TIME_TO_READ_MSGBOX))
			  
			  ||
				// hay opciones y la opción seleccionada es -1
			  ((_szMessageOptions != NULL) && (iRes == -1))
			);

	// Close the box.
	do {

		CFECore::TRC::CheckStep();
		
	} while(! CFECore::TRC::bCloseMessageBox());

}
// ----------------------------------------------------------------------------
void CFECore::Profile::ShowMessage(const char* _szMessage,const char* _szMessageOptions)
{
	FETick oTicks = CFECore::oGetEngineTicks();	
	OpenMessageBox(0,oTicks,_szMessage,_szMessageOptions,true);
	WaitAndCloseMessageBox(0,oTicks,_szMessage,_szMessageOptions,true);
}
// ----------------------------------------------------------------------------
void Encrypt(unsigned char* _pucDstData,unsigned char* _pucSrcData, uint _uiSrcSize)
{
	for (uint i=0;i<_uiSrcSize;i++)
		_pucDstData[i] = _pucSrcData[i] ^ 0xa5;
}
// ----------------------------------------------------------------------------
void Decrypt(unsigned char* _pucDstData,unsigned char* _pucSrcData, uint _uiSrcSize)
{
	Encrypt(_pucDstData,_pucSrcData,_uiSrcSize);
}
// ----------------------------------------------------------------------------
void CFECore::Profile::BuildSaveData(FEPointer _pBuffer,uint _uiBuffSize,FEPointer _pData, uint _uiSize)
{
	FEPointer pNewData = CFECore::Mem::pAlloc(_uiSize + CRC_SIZE);

		// compute crc of the save data
		uint uiCalcCRC = CFEStringUtils::uiGetCRC32((const char*)_pData,_uiSize);

		// store the crc of values into the new data buffer
		memcpy(pNewData,&uiCalcCRC,CRC_SIZE);

		// store save data into the new data buffer
		memcpy(pNewData+CRC_SIZE,_pData,_uiSize);

		// Encrypt data including the computed CRC
		Encrypt(_pBuffer + ENCRIPT_DATA_OFFSET, pNewData, _uiSize + CRC_SIZE);

		// setup id in the destination buffer
		memcpy(_pBuffer,&PROFILEID, PROFILEID_SIZE);

		// setup application profile version
		memcpy(_pBuffer+PROFILEID_SIZE,&FECglobals.m_uiApplicationVersion, VERSION_SIZE);

	CFECore::Mem::Free(pNewData);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Profile::bDumpSaveData(FILE* fd,FEPointer _pData, uint _uiSize)
{
	// Create data to store into the new file
	uint uiTotalSize = PROFILEID_SIZE + VERSION_SIZE + CRC_SIZE + _uiSize;
	if (uiTotalSize<512) uiTotalSize = 512;

	FEPointer pNewData = CFECore::Mem::pAlloc(uiTotalSize);

		// process the data
		BuildSaveData(pNewData,uiTotalSize,_pData,_uiSize);

		// Write to file
		uint uiRes = fwrite(pNewData,1,uiTotalSize,fd);

	CFECore::Mem::Free(pNewData);
	return (uiRes == uiTotalSize);
}
// ----------------------------------------------------------------------------
/// Saves data to the application profile assigned area.
FEBool CFECore::Profile::bSave(FEPointer _pData, uint _uiSize)
{
	// ------------------------------------------------------
	// BEGIN: Save process
	// ------------------------------------------------------
	FEBool bRes = false;
	FETick oTicks = CFECore::oGetEngineTicks();	
	OpenMessageBox(_uiSize,oTicks,GENERIC_SAVE_MESSAGE,GENERIC_SAVE_MESSAGE_OPTIONS,false);

		CFEString sFilename = sGetProfileFilename(FECglobals.m_sApplicationName);
		FILE* fd = NULL;
		fopen_s(&fd,sFilename.szString(),"wb");

		if (fd != NULL)
		{
			bRes = bDumpSaveData(fd,_pData,_uiSize);
			fclose(fd);
		}

	// Wait enough time to let the user read the text or press some option ...
	WaitAndCloseMessageBox(_uiSize,oTicks,GENERIC_SAVE_MESSAGE,GENERIC_SAVE_MESSAGE_OPTIONS,false);
	// ------------------------------------------------------
	// END: Save process
	// ------------------------------------------------------

	return(bRes);
}
// ----------------------------------------------------------------------------
// PRE: Filesystem should be mounted.
// ----------------------------------------------------------------------------
FEBool CFECore::Profile::bCreateFileStructure(FEPointer _pData, uint _uiSize)
{
	FEBool bRes = false;

		CFEString sFilename = sGetProfileFilename(FECglobals.m_sApplicationName);
		FILE* fd = NULL;
		fopen_s(&fd,sFilename.szString(),"wb");

		if (fd != NULL)
		{
			bRes = bDumpSaveData(fd,_pData,_uiSize);
			fclose(fd);
		}

	return(bRes);
}
// ----------------------------------------------------------------------------
/// Creates the save data for the first time.
// ----------------------------------------------------------------------------
FEBool CFECore::Profile::bCreateSaveData(FEPointer _pData, uint _uiSize,FEBool _bForceShowMessage)
{
	// ------------------------------------------------------
	// BEGIN: Create Save data process
	// ------------------------------------------------------
	FETick oTicks = CFECore::oGetEngineTicks();	
	OpenMessageBox(_uiSize,oTicks,CREATE_SAVE_MESSAGE,CREATE_SAVE_MESSAGE_OPTIONS,_bForceShowMessage);

		// Show Message Box?
		FEBool result = bCreateFileStructure(_pData,_uiSize);

	WaitAndCloseMessageBox(_uiSize,oTicks,CREATE_SAVE_MESSAGE,CREATE_SAVE_MESSAGE_OPTIONS,_bForceShowMessage);
	// ------------------------------------------------------
	// END: Create save data process
	// ------------------------------------------------------

    return result;
}
// ----------------------------------------------------------------------------
FEBool CFECore::Profile::bRecoverFileSystem(FEPointer _pData, uint _uiSize,FEBool _bForceShowMessage)
{
	FETick oTicks = CFECore::oGetEngineTicks();	
	OpenMessageBox(_uiSize,oTicks,BADFSFORMAT_ERROR_MESSAGE,BADFSFORMAT_ERROR_MESSAGE_OPTIONS,_bForceShowMessage);

		FEBool result = bCreateFileStructure(_pData,_uiSize);
	
	WaitAndCloseMessageBox(_uiSize,oTicks,BADFSFORMAT_ERROR_MESSAGE,BADFSFORMAT_ERROR_MESSAGE_OPTIONS,_bForceShowMessage);
	return result;
}
// ----------------------------------------------------------------------------
/// Loads the data from the application profile assigned area.
FEBool CFECore::Profile::bLoad(FEPointer _pData, uint _uiSize)
{
	CFEString sFilename = sGetProfileFilename(FECglobals.m_sApplicationName);

	// Open file
	FILE* fd = NULL;
	fopen_s(&fd,sFilename.szString(),"rb");
	if (fd == NULL)
	{
		return( bCreateSaveData(_pData,_uiSize,true) );
	}

    // Load file
	uint uiTotalSize = PROFILEID_SIZE + VERSION_SIZE + CRC_SIZE + _uiSize;

	if (uiTotalSize<512) uiTotalSize = 512;
	FEPointer pLoadedData = CFECore::Mem::pAlloc(uiTotalSize);

    FEBool res = false;
    uint uiReadBytes = fread(pLoadedData,1,uiTotalSize,fd);

    if(uiReadBytes != uiTotalSize)
    {
		// data corruption?
		fclose(fd);
		CFECore::Mem::Free(pLoadedData);

		return( bRecoverFileSystem(_pData,_uiSize,true) );
    }

	TProfileHeader* poProfileHeader = (TProfileHeader*)pLoadedData;

	int iVal = memcmp(poProfileHeader->m_ucID,PROFILEID,PROFILEID_SIZE);
	if (iVal != 0)
	{
		// data corruption?
		fclose(fd);
		CFECore::Mem::Free(pLoadedData);

		return( bRecoverFileSystem(_pData,_uiSize,true) );
	}

	/// Retrieve application version
	if (poProfileHeader->m_uiVersion != FECglobals.m_uiApplicationVersion)
	{
		// Application version and profile version mismatch!
		fclose(fd);
		CFECore::Mem::Free(pLoadedData);

		return( bRecoverFileSystem(_pData,_uiSize,true) );
	}

	// ok guay, vamos a desencriptar
	FEPointer pNewData = CFECore::Mem::pAlloc(_uiSize+CRC_SIZE);
	Decrypt(pNewData, pLoadedData + ENCRIPT_DATA_OFFSET,_uiSize + CRC_SIZE);

	// retrieve the crc from the decrypted buffer
	uint uiSavedCRC = *(uint *)(pNewData);

	// and compute crc of stored values...
	uint uiCalcCRC = CFEStringUtils::uiGetCRC32((const char*)pNewData + CRC_SIZE,_uiSize);
	if (uiSavedCRC != uiCalcCRC)
	{
		// data corruption?
		fclose(fd);
		CFECore::Mem::Free(pNewData);
		CFECore::Mem::Free(pLoadedData);

		return( bRecoverFileSystem(_pData,_uiSize,true) );
	}	
	fclose(fd);

	// ok let's copy the contents of the decrypted buffer into the destination buffer
	memcpy(_pData,pNewData + CRC_SIZE,_uiSize);

	CFECore::Mem::Free(pNewData);
	CFECore::Mem::Free(pLoadedData);

    return(true);
}
// ----------------------------------------------------------------------------
void CFECore::Profile::Reset()
{
}
// ----------------------------------------------------------------------------
void CFECore::Profile::Finish()
{
}
// ----------------------------------------------------------------------------
uint CFECore::Profile::uiGetVersion()
{
	CFEString sFilename = sGetProfileFilename(FECglobals.m_sApplicationName);

	// Open file
	FILE* fd = NULL;
	fopen_s(&fd,sFilename.szString(),"rb");
	if (fd == NULL) return(0xffffffff);

    // Load file
    TProfileHeader oProfileHeader;
    uint uiReadBytes = fread(&oProfileHeader,1,sizeof(TProfileHeader),fd);
    fclose(fd);

    if (uiReadBytes != sizeof(TProfileHeader))
	{
		// possible error
		return(0xffffffff);
	}

	if (memcmp(&oProfileHeader.m_ucID,PROFILEID,PROFILEID_SIZE) != 0)
	{
		return(0xffffffff);
	}

	return(oProfileHeader.m_uiVersion);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
