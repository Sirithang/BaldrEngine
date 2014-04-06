#include "gfx/gfxbuffer.h"

#include <memory>

using namespace gfx;
using namespace gfx::gfxbuffer;

void (*gfxbuffer::impl::createImpl)(GFXBufferType type, GFXBufferUsage usage, void* data, uint32 initialSize) = 0;
void (*gfxbuffer::impl::destroyImpl)(ID buffer) = 0;
void (*gfxbuffer::impl::bindImpl)(ID buffer) = 0;
void (*gfxbuffer::impl::updateImpl)(ID buffer, void* data, uint32 size) = 0;

ObjectManager<GFXBuffer, MAX_BUFFER_OBJECT> gfxbuffer::_gManager;

ID gfxbuffer::create(GFXBufferType type,  GFXBufferUsage usage, void* data, uint32 initialSize, bool localStorage)
{
	GFXBuffer& buffer= _gManager.addAndGet();
	buffer._type = type;
	buffer._size = initialSize;
	buffer._data = 0;
	buffer._usage = usage;

	if(localStorage)
	{
		buffer._data = malloc(initialSize);
		memcpy(buffer._data, data, initialSize);
	}

	impl::createImpl(type, usage, data, initialSize);

	return buffer.id;
}

void gfxbuffer::destroy(ID buffer)
{
	GFXBuffer& inst= _gManager.lookup(buffer);

	if(inst._data != 0)
	{
		free(inst._data);
	}

	impl::destroyImpl(buffer);

	inst._size = 0;
	_gManager.remove(buffer);
}

void gfxbuffer::bind(ID buffer)
{
	impl::bindImpl(buffer);
}

void gfxbuffer::update(ID buffer, void* data, uint32 size)
{
	impl::updateImpl(buffer, data, size);
}