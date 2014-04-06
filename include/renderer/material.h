#pragma once

#include "core/constants.h"
#include "core/objectmanager.h"

namespace baldr
{
	struct MaterialParamater
	{
		char*	name;
		//offset in the buffer to that particular Param
		uint32	offset;
	};

	//association of a shader and its constant value
	struct Material : ManagedObject
	{
		ID _shader;
		ID _constantBuffer;
		MaterialParamater params[32];
	};
	
	namespace material
	{
		extern ObjectManager<Material, MAX_MATERIALS_OBJECTS> _gManager;

		void create(ID shader);
		void destroy(ID material);
	}
}