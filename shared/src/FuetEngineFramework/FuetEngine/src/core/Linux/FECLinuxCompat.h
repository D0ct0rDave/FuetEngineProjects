// ----------------------------------------------------------------------------
/*! \brief Compatibility layer for code written against the MSVC runtime.
 *
 *  The engine, the samples and the games use the MSVC "secure" CRT variants
 *  (sprintf_s, fopen_s, ...). glibc does not provide them.
 *
 *  This header is not included from the sources: the Linux premake scripts
 *  force-include it into every translation unit (premake: forceincludes).
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FECLinuxCompatH
#define FECLinuxCompatH
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
// ----------------------------------------------------------------------------
#ifdef __cplusplus

#define sprintf_s(buf,size,...) snprintf((buf),(size),__VA_ARGS__)
#define vsprintf_s(buf,size,fmt,args) vsnprintf((buf),(size),(fmt),(args))

static inline int fopen_s(FILE** _ppFile,const char* _szName,const char* _szMode)
{
	*_ppFile = fopen(_szName,_szMode);
	return( (*_ppFile != NULL)?0:errno );
}

#endif
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
