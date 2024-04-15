// ----------------------------------------------------------------------------
/*! \class CFEArray
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 *  \par This string implementation comes directle from the GammaE Engine.
 */
// ----------------------------------------------------------------------------
#ifndef CFEArrayH
#define CFEArrayH
// ----------------------------------------------------------------------------
#include <vector>
#include "support/mem/CFEMem.h"
// ----------------------------------------------------------------------------
template <typename T>
class CFEArray : public std::vector<T>
{
    public:

        /// Deletes the element in the array specified by the given index.
        void Delete(uint _uiElement)
        {
            this->erase( this->begin() + (long)_uiElement);
        }        

        /// Deletes the element in the array specified by the given index.
        void push_back(const T& _oElem)
        {
			// make internal vector allocations using system allocation.
            std::vector<T>::push_back(_oElem);
        }
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
