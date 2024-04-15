// ----------------------------------------------------------------------------
/*! \class CFEHashTable
 *  \brief Hash table storage class.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par GammaE License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHashTableH
#define CFEHashTableH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Support/Mem/CFEMem.h"
#include "Support/Misc/CFEStringUtils.h"
#include "types/CFELookupDatabase.h"
// ----------------------------------------------------------------------------
template <class T>
class CFEHashTable
{
    public:
        ///
        CFEHashTable() : m_poElems(NULL), m_uiNumCols(0), m_uiNumRows(0)
        {
        };

        CFEHashTable(uint _uiNumRows,uint _uiNumCols)
        {
            Init(_uiNumRows,_uiNumCols);
        };

        ~CFEHashTable()
        {
            Finish();
        };

        void Init (uint _uiNumRows, uint _uiNumCols)
        {
            Finish();

            m_uiNumCols = _uiNumCols;
            m_uiNumRows = _uiNumRows;
            m_poElems = new CFELookUpDatabase<T>[_uiNumCols];
        }

        /// Adds a variable to the hash table
        void AddVar (const CFEString& _sString, T* _oValue)
        {
            int iIndex = uiGetIndex(_sString);
            m_poElems[iIndex].uiAdd(_sString,_oValue);
        }

        /// Deletes a variable associated with the given hash value
        void DeleteVar(const CFEString& _sString)
        {
            int iIndex = uiGetIndex(_sString);
			int iIdx = m_poElems[iIndex].iGetIdx(_sString);
			if (iIdx>-1) m_poElems[iIndex].Delete(iIdx);
        }

        /// Retrieves a variable from the hash table
        T* poGetVar (const CFEString& _sString) const
        {
            int iIndex = uiGetIndex(_sString);
            return ( m_poElems[iIndex].poGet(_sString) );
        }

        /// Adds a variable to the hash table usign a custom hash value
        void AddVar (uint _uiHash, T* _oValue)
        {
            int iIndex = _uiHash % m_uiNumCols;
            m_poElems[iIndex].uiAdd(_uiHash,_oValue);
        }

        /// Deletes a variable associated with the given hash value
        void DeleteVar(uint _uiHash)
        {
			int iIndex = _uiHash % m_uiNumCols;
			int iIdx = m_poElems[iIndex].iGetIdx(_uiHash);
			if (iIdx>-1) m_poElems[iIndex].Delete(iIdx);
        }

        void Finish ()
        {
            if (m_poElems != NULL)
            {
                delete []m_poElems;
                m_poElems = NULL;
            }
        }

        /// Invalidates the contents of the hash table.
        void Invalidate ()
        {
            Finish();
            Init(m_uiNumRows,m_uiNumCols);
        }

        uint uiNumRows() { return(m_uiNumRows); };
        uint uiNumCols() { return(m_uiNumCols); };

        CFELookUpDatabase<T>* poGetList(uint _uiCol)
        {
            return( &m_poElems[_uiCol] );
        };

    protected:

        CFELookUpDatabase<T>* m_poElems;

    private:

        uint uiGetIndex (const CFEString& _sString) const
        {
            return( CFEStringUtils::uiGetHash(_sString) % m_uiNumCols );
        }

        uint m_uiNumCols;
        uint m_uiNumRows;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
