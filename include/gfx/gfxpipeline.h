#pragma once

#include "core/constants.h"
#include "gfx/gfxdevice.h"
#include "core/objectmanager.h"

namespace gfx
{
	enum ShaderType
	{
		VERTEX_SHADER = 0x1,
		PIXEL_SHADER = 0x2
	};


	struct GFXPipelineConstant
	{
		char	name[32];
		uint8	offset;
		uint8	size;//no type can exceed 256B anyway
	};

	/**
	* GFXPipeline define a rendering pipeline. In pratice it's mostly
	* a set of shader for the different stage + associated binding info
	* like constant buffer. Each impl. must handle things internally
	* reading from an all purpose Shader file
	*/
	struct GFXPipeline : public ManagedObject
	{
		uint32 _typecontains;
		GFXPipelineConstant _variables[MAX_PIPELINE_CONSTANTES];
	};

	namespace gfxpipeline
	{
		extern ObjectManager<GFXPipeline, MAX_PIPELINE_OBJECT> _gManager;

		ID create();
		void destroy(ID shader);

		//It will use whatever it find in source to initialize the shader.
		void initFromSource(ID shader, const char* path);

		void uploadBuffer(ID buffer, uint8 slot);

		void bind(ID shader);

		namespace impl
		{
			//NOTE : the impl create don't return an ID. That's because it will be the same than
			//the generic one (they are created/destroy together
			extern void (*createImpl)();
			extern void (*destroyImpl)(ID shader);

			extern void (*initFromSourceImpl)(ID shader, const char* pPath);
			extern void (*bindImpl)(ID shader);

			extern void (*uploadBufferImpl)(ID buffer, uint8 slot);
		}
	}
}