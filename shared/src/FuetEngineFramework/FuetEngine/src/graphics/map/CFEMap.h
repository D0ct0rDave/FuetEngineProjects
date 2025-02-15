// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMapH
#define CFEMapH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
#include "types/CFEColor.h"
#include "types/CFEString.h"
#include "types/CFENamedObject.h"
#include "Graphics/Sprite/CFESprite.h"
// ----------------------------------------------------------------------------
class CFEMapElement
{
    public:
    	CFEMapElement() : m_bVisible(true), m_rDepth(_0r), m_rAngle(0), m_hSprInst(NULL)
    	{
    		
    	}
		
		/// Should this element be rendered at a given moment?
		bool	  m_bVisible;

        /// Position of this element related to the layer origin
        CFEVect2  m_oPos;

        /// Scale of this element
        CFEVect2  m_oScale;

        /// Depth of this element
        FEReal    m_rDepth;
        
        /// Angle of this element
        FEReal    m_rAngle;

        /// Tint color of this element
        CFEColor  m_oColor;

        /// The "shape" of this element
        FEHandler m_hSprInst;
};
// ----------------------------------------------------------------------------
class CFEMapSector
{
    public:
		
		/// Default constructor of the class.    
    	CFEMapSector() : m_bVisible(true)
    	{
    		
    	}

    	/// Destructor for the class.
    	~CFEMapSector();

        /// The complete list of the elements of this sector / should be arranged spatially and by material.
        CFEArray <CFEMapElement> m_oElements;

        /// The Sector bounding volume.
        CFERect	m_oBV;

        /// Is this layer visible or does it contain non renderable info.
        bool m_bVisible;
};
// ----------------------------------------------------------------------------
class CFEMapLayer : public CFENamedObject
{
    public:
		
		/// Default constructor of the class.    
    	CFEMapLayer() : CFENamedObject(""), m_oParallaxFact(CFEVect2::ZERO()), m_rDepth(_0r), m_oSpeed(CFEVect2::ZERO()), m_bAutoMovable(false), m_bVisible(true)
    	{
    		
    	}
    	
    	/// Destructor for the class.
    	~CFEMapLayer();

		/// The list of sectors that make this layer.
        CFEArray<CFEMapSector*> m_poSectors;

        /// Parallax factor of the layer.
        CFEVect2  m_oParallaxFact;

        /// Depth of the layer
        FEReal    m_rDepth;

        /// Movement (speed) of the layer.
        CFEVect2  m_oSpeed;
        
        /// Does this layer move automatically?
        bool      m_bAutoMovable;
        
        /// Is this layer visible or does it contain non renderable info.
        bool      m_bVisible;
};
// ----------------------------------------------------------------------------
class CFEMap : public CFENamedObject
{
    public:

		/// Default constructor of the class.
		CFEMap() : CFENamedObject("")
		{
			
		}
		
		/// Destructor for the class.
		~CFEMap();

        /// Retrieves the layer associated with the given name.
        CFEMapLayer* poGetLayer(const CFEString& _sLayerName);

        /// The list of layers belonging to this map.
        CFEArray <CFEMapLayer*> m_poLayers;

        /// The map bounding volume.
        CFERect	m_oBV;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
