// include the basic windows header files and the Direct3D header files

#include "core/windowing.h"
#include "core/engine.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")


// the entry point for any Windows program
int main(int argc, char** argv)
{
	baldr::WinHandle* win = baldr::window::createWindow(argc, argv);
	baldr::window::showWindow(win);

	if(baldr::engine::init(win) == FALSE)
	{
		baldr::engine::shutdown();
		return -1;
	}

	while(win->_opened)
    {
		baldr::window::updateWindow(win);

		baldr::engine::update();
    }

	baldr::engine::shutdown();
}