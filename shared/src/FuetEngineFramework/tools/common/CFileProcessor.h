//-----------------------------------------------------------------------------
/*! \class CFileProcessor
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef CFileProcessorH
#define CFileProcessorH
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
class CFileProcessor
{
	public:
			
		/// Default constructor for the class. Set's up the default options of the processor
		CFileProcessor();

		/// Processes the files of a directory.
		void ProcessFiles(const std::string& _sFindStr);

		/// Parses the command line and sets up options
		virtual void ParseCommandLine(int argc, char* argv[]);

	protected:

		/// Called when the processor starts its job.
		virtual void StartProcess();

		/// Called when the processor finishes its job.
		virtual void EndProcess();

		/// Called when the processor enters a new directory.
		virtual void EnterDir(const std::string& _sDirectory);

		/// Called every time a new file is found and should be processed.
		virtual void Process(const std::string& _sFilename);

		///	Shows the usage message of the file processor.
		virtual void ShowUsage();
		
		/// Show a given error message and exists the application.
		virtual void Error(int _iError,char* arg = NULL);

		/// 
		void ProcessFiles_INT(const std::string& _sFindStr);

	protected:

		int m_iRequiredParams;

		bool m_bRecursive;
		
		///
		std::string m_sInputFilename;
		
		///
		std::string m_sOutputFilename;		
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
