// ----------------------------------------------------------------------------
/*! \class CFEArray
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
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
			EFEMemAllocPolicy eOldPolicy =CFEMem::eGetMemPolicy();
			CFEMem::SwitchPolicy(MP_SYSTEM_ALLOCATION);

            this->erase( this->begin() + _uiElement);
            this->resize(this->size());

            CFEMem::SwitchPolicy(eOldPolicy);            
        }        

        /// Deletes the element in the array specified by the given index.
        void push_back(const T& _oElem)
        {
			// make internal vector allocations using system allocation.
			/*
			if (this->capacity() < (this->size()+1))
			{
				
				EFEMemAllocPolicy eOldPolicy =CFEMem::eGetMemPolicy();
				CFEMem::SwitchPolicy(MP_SYSTEM_ALLOCATION);
				this->reserve(this->size()+1);

	            CFEMem::SwitchPolicy(eOldPolicy);
	        }
			*/

            std::vector<T>::push_back(_oElem);
        }
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
