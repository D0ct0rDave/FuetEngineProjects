// ----------------------------------------------------------------------------
/*! \class CFELogger
 *  \brief Logging racilities
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFELoggerH
#define CFELoggerH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CFELogger
{
    public:

		/// Default constructor for the logger. Specify the size of the logging buffer or 0 to use the
		/// system logger.
		CFELogger(uint _uiBufferSize=0);

        /// Prints a message to the log.
        void Print(char *fmt,...);

        /// Writes an warning to the log.
        void Warning(char *fmt,...);

        /// Writes an error to the log.
        void Error(char *fmt,...);

        /// Retrieves the log buffer pointer or NULL if the system logger is being used.
        const char* szGetLog();

        /// Cleans the contents of the logger.
        void Clean();

    protected:
        
        /// Adds a message to the logger.
        virtual void AddMessage(char* _szMessage);
        
        /// Buffer to store the log
        char* m_pucBuffer;
        uint  m_uiBufferSize;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
