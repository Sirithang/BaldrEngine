#pragma once

#include "core/constants.h"
#include "gfx/gfxdevice.h"
#include "core/objectmanager.h"

namespace gfx
{
	enum GFXBufferType
	{
		VERTEX_BUFFER = 0x1,
		INDEX_BUFFER = 0x2,
		CONSTANT_BUFFER = 0x3
	};

	enum GFXBufferUsage
	{
		DEFAULT = 0x0,
		IMMUTABLE = 0x1,
		DYNAMIC = 0x2
	};

	struct GFXBuffer : ManagedObject
	{
		GFXBufferType _type;
		GFXBufferUsage _usage;
		//total size. e.g. if contains 10 elem of 4 byte, size == 40
		uint32 _size;
		//the user have the responsability of accessing that buffer right.
		void* _data;
	};

	namespace gfxbuffer
	{
		extern ObjectManager<GFXBuffer, MAX_BUFFER_OBJECT> _gManager;

		ID create(GFXBufferType type, GFXBufferUsage usage, void* data, uint32 initialSize, bool localStorage = false);
		void destroy(ID buffer);

		void bind(ID buffer);

		void update(ID buffer, void* data, uint32 size);

		namespace impl
		{
			//as there is a 1:1 called between impl & generic, the ID will always match.
			//unmatching ID is a sign of major frell up.
			extern void (*createImpl)(GFXBufferType type, GFXBufferUsage usage, void* data, uint32 initialSize);
			extern void (*destroyImpl)(ID buffer);

			extern void (*bindImpl)(ID buffer);

			extern void (*updateImpl)(ID buffer, void* data, uint32 size);
		}
	}
}