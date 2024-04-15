// ----------------------------------------------------------------------------
/*! \class CFESystem
 *  \brief Logging racilities
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESystemH
#define CFESystemH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEMomentInTime.h"
// ----------------------------------------------------------------------------
class CFESystem
{
	public:
		
		/// Retrieves the current moment in time.
		static CFEMomentInTime oGetMomentInTime();
	
		// -------------------
		// Parallel Processing
		// -------------------
		typedef void(*TProcessFunc)(FEPointer _pParam);

		/// Starts a process to be executed in parallel to the main process (thread)
		static FEHandler hStartProcess(TProcessFunc _pProcess,FEPointer _pParam);

		/// Tells whether a parallel process has finished its execution or not.
		static FEBool bIsProcessFinished(FEHandler _hProcess);

		/// Frees the resources allocated by an already finished process.
		static void EndProcess(FEHandler _hProcess);

		// -------------------
		// System Logging
		// -------------------
		typedef void(*TSystemLogFunc)(const char * _szLogMessage);

	#ifndef MASTER_RELEASE_VERSION

		/// Sets the system logging function to be called for logging purposes
		static void SetSystemLogFunc(TSystemLogFunc _pProcess);

		/// Prints a message to the log.
		static void Print(const char *fmt,...);

		/// Writes an warning to the log.
		static void Warning(const char *fmt,...);

		/// Writes an error to the log.
		static void Error(const char *fmt,...);

	#else

		/// Sets the system logging function to be called for logging purposes
		// static void SetSystemLogFunc(TSystemLogFunc _pProcess){};
		static void SetSystemLogFunc(TSystemLogFunc);

		/// Prints a message to the log.
		static void Print(const char *,...);

		/// Writes an warning to the log.
		static void Warning(const char *,...);

		/// Writes an error to the log.
		static void Error(const char *,...);

	#endif
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
