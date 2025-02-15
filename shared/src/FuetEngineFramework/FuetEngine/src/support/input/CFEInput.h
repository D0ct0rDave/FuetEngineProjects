// ----------------------------------------------------------------------------
/*! \class CFEInput
 *  \brief High Level Input Management Class
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEInputH
#define CFEInputH
// ----------------------------------------------------------------------------
#include "FEEnums.h"
#include "FEBasicTypes.h"
#include "Types/CFEVect2.h"
// ----------------------------------------------------------------------------
class CFEInputData;
// ----------------------------------------------------------------------------
class CFEInput
{
	public:

        /// Constructor of the input manager
        CFEInput();

        /// Constructor of the input manager
        CFEInput(FEHandler _hParam) { Init(_hParam); };

        /// Destructor of the class
        ~CFEInput();

        /// Initializes the input manager
        void Init(FEHandler _hParam);
		
		/// Finalizes the input manager.
        void Finish();

		/// Updates the input data.
		void Update(FEReal _rDeltaT);

		/// Retrieve down state for the given button.
		bool bDown(EFEInputButton _eIB);

		/// Retrieve up state for the given button.
		bool bUp(EFEInputButton _eIB);

		/// Retrieve pressed state for the given button.
		bool bPressed(EFEInputButton _eIB);

		/// Retrieve release state for the given button.
		bool bReleased(EFEInputButton _eIB);

		/// Retrieve value for the given axis.
		FEReal rValue(EFEInputAxis _eIA);

		/// Sets the screen virtual width.
		void SetScreenVWidth(uint _uiScrVWidth);

		/// Sets the virtual screen height.
		void SetScreenVHeight(uint _uiScrVHeight);

		/// Retrieve cursor coords in virtual screen coordinates.
		const CFEVect2& oCursorCoords();

		/// Retrieve the pressure of a given input pressure button.
		FEReal rCursorPressure(EFEInputPressureButton _eIPB);

		/// Retrieve down state for the given button.
		bool bDown(EFEInputPressureButton _eIPB);

		/// Retrieve up state for the given button.
		bool bUp(EFEInputPressureButton _eIPB);

		/// Retrieve pressed state for the given button.
		bool bPressed(EFEInputPressureButton _eIPB);

		/// Retrieve release state for the given button.
		bool bReleased(EFEInputPressureButton _eIPB);

    private:

		/// Input manager Internal Data.
        CFEInputData* m_poData;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
