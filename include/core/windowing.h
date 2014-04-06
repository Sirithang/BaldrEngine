#pragma once

namespace baldr
{
	struct WinHandle
	{
		//native handle for the window. Allow to be cast to nativ pointer.
		void* nativeHandle;
		bool _opened;
	};

	namespace window
	{
		extern WinHandle* (*createWindow)(int, char**);
		extern void (*showWindow)(WinHandle*);

		extern void (*updateWindow)(WinHandle*);
	}
}