#pragma once

#include "windowing.h"
#include "renderer/renderer.h"

namespace baldr
{
	struct Engine
	{
		Renderer _renderer;
	};

	namespace engine
	{
		extern Engine gEngine;

		uint8_t init(baldr::WinHandle* window);
		void update();
		void shutdown();
	}
}