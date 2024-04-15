// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
	            CFEHUDLabel(const CFEString& _sName) : 
					CFEHUDObject(_sName), 
					m_poFont(NULL), 
					m_eHAlignment(THAM_NONE), 
					m_eVAlignment(TVAM_NONE),
					m_rPointSize(_1r),
					m_rTracking(_0r),
					m_rInterlining(_0r),
					m_rAdjustmentWidth(-_1r),
					m_sAdjustedText(CFEString::sNULL())
	            {

	            };

				/// Destructor of the class
				virtual ~CFEHUDLabel();

				/// Sets the font for this label.
				void SetFont(CFEFont* _poFont)
				{
					m_poFont = _poFont;

					if (m_rAdjustmentWidth != -_1r)
						AdjustText();
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

					if (m_rAdjustmentWidth != -_1r)
						AdjustText();
				}

				/// Sets the text to be displayed by this label, using a formated string.
				void SetText(const char *fmt,...);

				/// Retrieves the text of this label.
				const CFEString& sGetText()
				{
					return ( m_sText );
				}

				/// Retrieves the printable text string of this label.
				const CFEString& sGetPrintableText()
				{
					if (m_rAdjustmentWidth == -1)
						return ( m_sText );
					else
						return(m_sAdjustedText);
				}

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

				/// Sets the point size for this label. PointSize scales uniformly the text being displayed.
				void SetPointSize(FEReal _rPointSize)
				{
					m_rPointSize = _rPointSize;
					if (m_rAdjustmentWidth != -_1r)
						AdjustText();
				}

				/// Retrieves the point size for this label.
				FEReal rGetPointSize()
				{
					return( m_rPointSize );
				}

				/// Sets the tracking used by this label.
				void SetTracking(FEReal _rTracking)
				{
					m_rTracking = _rTracking;
					if (m_rAdjustmentWidth != -_1r)
						AdjustText();					
				}

				/// Retrieves the tracking used by this label.
				FEReal rGetTracking()
				{
					return( m_rTracking );
				}
				
				/// Sets the interlining space used by this label.
				void SetInterlining(FEReal _rInterlining)
				{
					m_rInterlining = _rInterlining;
				}

				/// Retrieves the interlining space used by this label.
				FEReal rGetInterlining()
				{
					return( m_rInterlining );
				}

        		/// Retrieves the adjustment width this label will accomodate. -1 if no adjustment width
        		FEReal rGetAdjustmentWidth()
        		{
        			return(m_rAdjustmentWidth);
        		}

        		/// Sets the adjustment width this label will accomodate. -1 if no adjustment width.
        		void SetAdjustmentWidth(FEReal _rAdjustmentWidth)
        		{
        			m_rAdjustmentWidth = _rAdjustmentWidth;
					if (m_rAdjustmentWidth != -_1r)
						AdjustText();
        		}

                /// Perform processing over the object
        		virtual void Accept(CFEHUDVisitor* _poVisitor)
        		{
        		    _poVisitor->Visit(this);
        		}

	protected:
				void AdjustText();

				CFEFont*		        m_poFont;
				CFEString		        m_sText;
				EFETextHAlignmentMode   m_eHAlignment;
				EFETextVAlignmentMode   m_eVAlignment;
				FEReal					m_rPointSize;
				FEReal					m_rTracking;
				FEReal					m_rInterlining;
	
				FEReal					m_rAdjustmentWidth;
				CFEString		        m_sAdjustedText;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
