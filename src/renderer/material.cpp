#include "renderer/material.h"

#include "gfx/gfxbuffer.h"

using namespace baldr;

ObjectManager<Material, MAX_MATERIALS_OBJECTS> baldr::material::_gManager;

void material::create(ID shader)
{
	Material& mat = _gManager.addAndGet();

	mat._shader = shader;

	mat._constantBuffer = gfx::gfxbuffer::create(gfx::CONSTANT_BUFFER, gfx::DEFAULT, 0, 10);
}