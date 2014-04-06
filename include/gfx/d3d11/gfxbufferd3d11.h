#pragma once

#ifdef D3D11RENDERER

#include"gfx/gfxbuffer.h"

#include <windows.h>
#include <d3d11.h>

namespace gfx
{
	struct GFXBufferD3D11 : public ManagedObject
	{
		ID3D11Buffer* _nativeBuffer;
	};


	namespace gfxbufferd3d11
	{
		extern ObjectManager<GFXBufferD3D11, MAX_BUFFER_OBJECT> _gManager;

		void create(GFXBufferType type, GFXBufferUsage usage, void* data, uint32 initialSize);
		void destroy(ID buffer);

		void bind(ID buffer);

		void update(ID buffer, void* data, uint32 size);

		UINT GFXToD3D11BindFlags(gfx::GFXBufferType type);
	}
}

#endif