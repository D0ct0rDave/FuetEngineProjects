// ----------------------------------------------------------------------------
#ifndef CColorListH
#define CColorListH
// ----------------------------------------------------------------------------
#include <vector>
// ----------------------------------------------------------------------------
// A class to keep count of the number of colors of an image.
// ----------------------------------------------------------------------------
class CColorList
{
	public:
		CColorList()
		{
			m_uiMaxColors = 256;
		}

		CColorList(unsigned int _uiMaxColors)
		{
			m_uiMaxColors = _uiMaxColors;
		}
		
		/// Retrieves the maximum number of different colors this list can hold.
		unsigned int uiGetMaxColors()
		{
			return(m_uiMaxColors);
		}

		/// Adds a color to the color list in case it doesn't exist and size < 257
		unsigned int uiAdd(unsigned int _uiColor)
		{
			// look if the color exist in the list
			for (unsigned int i=0;i<m_uiColors.size();i++)
				if (m_uiColors[i] == _uiColor)
					return(i);

			if (m_uiColors.size() < m_uiMaxColors)
			{
			    // add the new color
			    m_uiColors.push_back(_uiColor);

			    return((unsigned int)m_uiColors.size()-1);
			}
		    else
			    return(m_uiMaxColors-1);
		}

		/// Retrieves the number of colors stored in the list.
		unsigned int uiSize()
		{
			return((unsigned int)m_uiColors.size());
		}

	private:

		std::vector<unsigned int> m_uiColors;
		unsigned int m_uiMaxColors;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
