#pragma once

#include "types.h"
#include "math_types.h"

namespace gfx
{
	struct GFXDevice
	{
	};

	extern GFXDevice* gDevice;

	enum GFXTopology
	{
		TOPOLGY_TRIANGLELIST,
	};

	namespace gfxdevice
	{
		//the void* should be cast according to implementation need (it must be handle in D3D11 case etc..)
		//return FALSE if an error awake (check log for it)
		extern uint8 (*create)(void* pWindow);
		extern void (*destroy)();

		extern void (*swap)();

		// Clear the currently set buffer
		extern void (*clear)(alfar::Vector4 pClearColor);

		extern void (*draw)(GFXTopology type, uint32 number, uint32 offset);

		//===================================================================================================
	}

	//---- add setup function for renderer here
	void setupD3D11Interface();
}