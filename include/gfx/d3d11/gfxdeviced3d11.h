#ifndef _GFX_GFXDEVICED3D11_H
#define _GFX_GFXDEVICED3D11_H

#ifdef D3D11RENDERER

#include "gfx/gfxdevice.h"

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

namespace gfx
{
	struct GFXDeviceD3D11 : public GFXDevice
	{
		ID3D11Device*           _device;
		ID3D11DeviceContext*    _context;
		IDXGISwapChain*         _swapchain;

		ID3D11RenderTargetView* _defaultColorbuffer;
	};

	//shortcut for immediatly d3d11device
	extern GFXDeviceD3D11* gD3D11Device;

	namespace gfxdeviced3d11
	{
		uint8 create(void* pWindow);
		void destroy();

		void swap();

		void clear(alfar::Vector4 pClearColor);

		void draw(GFXTopology type, uint32 number, uint32 offset);

		void setup();
	}
}

#endif

#endif