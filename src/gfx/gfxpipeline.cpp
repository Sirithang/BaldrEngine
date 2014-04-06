#include "gfx/gfxpipeline.h"

using namespace gfx;

ObjectManager<GFXPipeline, MAX_PIPELINE_OBJECT> gfxpipeline::_gManager;


void (*gfxpipeline::impl::createImpl)() = 0;
void (*gfxpipeline::impl::destroyImpl)(ID shader) = 0;
void (*gfxpipeline::impl::initFromSourceImpl)(ID shader, const char* pPath) = 0;
void (*gfxpipeline::impl::bindImpl)(ID shader) = 0;
void (*gfxpipeline::impl::uploadBufferImpl)(ID buffer, uint8 slot) = 0;

ID gfxpipeline::create()
{
	GFXPipeline& ret = _gManager.addAndGet();

	for(uint32 i = 0; i < MAX_PIPELINE_CONSTANTES; ++i)
	{
		ret._variables[i].size = 0;
		ret._variables[i].offset = 0;
		ret._variables[i].name[0] = '\0';
	}

	impl::createImpl();

	return ret.id;
}

void gfxpipeline::destroy(ID shader)
{
	_gManager.remove(shader);

	impl::destroyImpl(shader);
}

void gfxpipeline::initFromSource(ID shader, const char* source)
{
	impl::initFromSourceImpl(shader, source);
}

void gfxpipeline::bind(ID shader)
{
	impl::bindImpl(shader);
}

void gfxpipeline::uploadBuffer(ID buffer, uint8 slot)
{
	impl::uploadBufferImpl(buffer, slot);
}