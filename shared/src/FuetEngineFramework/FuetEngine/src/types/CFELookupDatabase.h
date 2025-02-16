//-----------------------------------------------------------------------------
#ifndef CFELookUpDatabaseH
#define CFELookUpDatabaseH
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T>
class CFELookUpDatabase
{
    public:
        /// Adds an element to the look up array (if it doens't exist) if it exists, modifies it.
        uint uiAdd(const CFEString& _sVariable,T* _poValue)
        {
            for (uint i=0;i<m_oData.size();i++)
                if (m_oData[i].m_sVariable == _sVariable)
                {
                    m_oData[i].m_poValue = _poValue;
                    return(i);
                }

            TResourceEntry oEntry;
            oEntry.m_poValue   = _poValue;
            oEntry.m_sVariable = _sVariable;

            m_oData.push_back(oEntry);
            return( m_oData.size()-1 );
        }

        /// Clears the full contents of the database
        void Clear()
        {
            m_oData.clear();
        }

        /// Returns an element from the look up array (if it exists)
        T* poGet(const CFEString& _sVariable)
        {
            for (uint i=0;i<m_oData.size();i++)
                if (m_oData[i].m_sVariable == _sVariable)
                    return(m_oData[i].m_poValue);

            return( NULL );
        }

        /// Returns the resource associated to the given index in the lookup array (if it exists).
        T* poGet(uint _uiIdx)
        {
            if (_uiIdx >= m_oData.size())
                return( NULL );
            else
                return( m_oData[_uiIdx].m_poValue );
        }

        /// Retrieves the variable name associated with the index of a resource.
        const CFEString& sGetVariable(uint _uiIdx)
        {
            if (_uiIdx >= m_oData.size())
                return( CFEString::sNULL() );
            else
                return( m_oData[_uiIdx].m_sVariable );
        }

        /// Retrieves the variable name associated to the given value.
        const CFEString& sGetVariable(T* _poValue)
        {
            for (uint i=0;i<m_oData.size();i++)
                if (m_oData[i].m_poValue == _poValue)
                    return(m_oData[i].m_sVariable);

            return( CFEString::sNULL() );
        }

        /// Retrieves the number of elements in the look up array
        uint uiNumElems()
        {
            return ( m_oData.size() );
        }

    protected:

        ///
        typedef struct
        {
            CFEString     m_sVariable;
            T*            m_poValue;

        }TResourceEntry;

        /// The database where all the elements are stored (can be a std::map)
        std::vector<TResourceEntry> m_oData;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
