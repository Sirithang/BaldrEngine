#include "gfx/gfxdevice.h"

gfx::GFXDevice* gfx::gDevice = 0;

uint8_t (*gfx::gfxdevice::create)(void* pWindow) = 0;
void (*gfx::gfxdevice::destroy)() = 0;
void (*gfx::gfxdevice::swap)() = 0;
void (*gfx::gfxdevice::clear)(alfar::Vector4 pClearColor) = 0;
void (*gfx::gfxdevice::draw)(GFXTopology type, uint32 number, uint32 offset);