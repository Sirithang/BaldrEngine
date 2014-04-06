#include "gfx/d3d11/gfxdeviced3d11.h"
#include "gfx/d3d11/gfxpipelined3d11.h"
#include "gfx/d3d11/gfxbufferd3d11.h"

void gfx::setupD3D11Interface()
{
	gfxdevice::create = gfx::gfxdeviced3d11::create;
	gfxdevice::destroy = gfx::gfxdeviced3d11::destroy;
	gfxdevice::clear = gfx::gfxdeviced3d11::clear;
	gfxdevice::swap = gfx::gfxdeviced3d11::swap;
	gfxdevice::draw = gfxdeviced3d11::draw;

	gfxpipeline::impl::createImpl = gfxpipelined3d11::create;
	gfxpipeline::impl::destroyImpl = gfxpipelined3d11::destroy;
	gfxpipeline::impl::initFromSourceImpl = gfxpipelined3d11::initFromSource;
	gfxpipeline::impl::bindImpl  = gfxpipelined3d11::bind;
	gfxpipeline::impl::uploadBufferImpl = gfxpipelined3d11::uploadBuffer;

	gfxbuffer::impl::createImpl = gfxbufferd3d11::create;
	gfxbuffer::impl::destroyImpl = gfxbufferd3d11::destroy;
	gfxbuffer::impl::bindImpl = gfxbufferd3d11::bind;
	gfxbuffer::impl::updateImpl = gfxbufferd3d11::update;
}