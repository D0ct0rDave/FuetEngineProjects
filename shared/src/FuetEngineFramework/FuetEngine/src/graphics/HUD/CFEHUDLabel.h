// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDLabelH
#define CFEHUDLabelH
//-----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "FEEnums.h"

#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEFont;
//-----------------------------------------------------------------------------
class CFEHUDLabel : public CFEHUDObject
{
	public:		
				/// Default constructor for the class.
	            CFEHUDLabel(const CFEString& _sName) : CFEHUDObject(_sName), m_poFont(NULL), m_eHAlignment(THAM_NONE), m_eVAlignment(TVAM_NONE)
	            {

	            };

				/// Destructor of the class
				virtual ~CFEHUDLabel();

				/// Sets the font for this label.
				void SetFont(CFEFont* _poFont)
				{
					m_poFont = _poFont;
				}

				/// Retrieves the font's label.
				CFEFont* poGetFont()
				{
					return( m_poFont );
				}

				/// Sets the text to be displayed by this label.
				void SetText(const CFEString& _sText)
				{
					m_sText = _sText;
				}

				/// Sets the text to be displayed by this label, using a formated string.
				void SetText(char *fmt,...);

				/// Sets the horizontal alignment for the label.
				void SetHAlignment(EFETextHAlignmentMode _eHAlignment)
				{
					m_eHAlignment = _eHAlignment;
				}

				/// Retrieves the horizontal alignment of the label.
				EFETextHAlignmentMode eGetHAlignment()
				{
					return( m_eHAlignment );
				}
				
				/// Sets the vertical alignment for the label.
				void SetVAlignment(EFETextVAlignmentMode _eVAlignment)
				{
					m_eVAlignment = _eVAlignment;
				}

				/// Retrieves the vertical alignment of the label.
				EFETextVAlignmentMode eGetVAlignment()
				{
					return( m_eVAlignment );
				}
				
				/// Retrieves the text of this label.
				const CFEString& sGetText()
				{
					return ( m_sText );
				}
                /// Perform processing over the object
        		virtual void Accept(CFEHUDVisitor* _poVisitor)
        		{
        		    _poVisitor->Visit(this);
        		}

	protected:
				CFEFont*		        m_poFont;
				CFEString		        m_sText;
				EFETextHAlignmentMode   m_eHAlignment;
				EFETextVAlignmentMode   m_eVAlignment;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------