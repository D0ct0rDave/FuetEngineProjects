// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEConfigFileH
#define CFEConfigFileH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEString.h"
// ----------------------------------------------------------------------------
class CFEConfigFile
{
    public:

        /// Default constructor for this class.
        CFEConfigFile();

        /// Creates the object and opens the configuration file ready for reading.
        CFEConfigFile(const CFEString& _sConfigFile);

        /// Default destructor.
        ~CFEConfigFile();

        /// Loads the configuration file ready for reading.
        FEBool bLoad(const CFEString& _sConfigFile);

        /// Retrieves whether the configuration file was correctly opened or not.
        FEBool bInitialized() const;

        /// Retrieves a string variable value from the configuration file
        CFEString sGetString(const CFEString& _sVar,const CFEString& _sDefaultValue) const;

        /// Retrieves a FEReal variable value from the configuration file
        FEReal rGetReal(const CFEString& _sVar,FEReal _rDefaultValue) const;

        /// Retrieves a integer variable value from the configuration file
        int iGetInteger(const CFEString& _sVar,int _iDefaultValue) const;

        /// Retrieves a FEBool variable value from the configuration file
        FEBool bGetBool(const CFEString& _sVar,FEBool _bDefaultValue) const;

        /// Retrieves whether the given variable exists or not.
        FEBool bExists(const CFEString& _sVar) const;

    protected:
        ///
        FEHandler   m_hConfigObj;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
