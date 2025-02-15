// -----------------------------------------------------------------------------
/*! \class CEntityFactory
 *  \brief This class creates game entities on demand.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CEntityFactoryH
#define CEntityFactoryH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
// -----------------------------------------------------------------------------
class CEntity;
// -----------------------------------------------------------------------------
class CEntityFactory
{
	public:

		/// Creates the entity associated to the given name.
		static CEntity* poCreate(const CFEString& _sEntity);
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
