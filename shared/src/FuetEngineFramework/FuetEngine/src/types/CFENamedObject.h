// ----------------------------------------------------------------------------
/*! \class CFENamedObject
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFENamedObjectH
#define CFENamedObjectH
//-----------------------------------------------------------------------------
#include "types/CFEString.h"
//-----------------------------------------------------------------------------
class CFENamedObject
{
	public:

        /// Default constructor of this object.
        CFENamedObject(const CFEString& _sName)
        {
            SetName(_sName);
        }

		/// Sets the name for this object.
		void SetName(const CFEString& _sName)
		{
		    m_sName = _sName;
		}

		/// Retrieves the name of this object.
		const CFEString sGetName()
		{
		    return ( m_sName );
		}

	protected:

		CFEString m_sName;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
