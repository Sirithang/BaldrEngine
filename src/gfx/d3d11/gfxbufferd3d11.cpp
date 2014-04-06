#include "gfx/d3d11/gfxdeviced3d11.h"

#include "gfx/d3d11/gfxbufferd3d11.h"
#include "core/logger.h"

#include <assert.h> 

using namespace gfx;
using namespace gfx::gfxbufferd3d11;

ObjectManager<GFXBufferD3D11, MAX_BUFFER_OBJECT> gfxbufferd3d11::_gManager;

void gfxbufferd3d11::create(GFXBufferType type,  GFXBufferUsage usage, void* data, uint32 initialSize)
{
	GFXBufferD3D11& buffer = gfxbufferd3d11::_gManager.addAndGet();
	buffer._nativeBuffer = 0;

    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = (D3D11_USAGE )usage;
	bd.ByteWidth = initialSize;
	bd.BindFlags = GFXToD3D11BindFlags(type);
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = data;

	HRESULT hr = gD3D11Device->_device->CreateBuffer( &bd, &InitData, &buffer._nativeBuffer );
    if( FAILED( hr ) )
	{
		baldr::logger::log("Error in buffer creation!");
	}
}

void gfxbufferd3d11::destroy(ID buffer)
{
	GFXBufferD3D11& inst = _gManager.lookup(buffer);

	if(inst._nativeBuffer != 0)
		inst._nativeBuffer->Release();
}

void gfxbufferd3d11::bind(ID buffer)
{
	GFXBufferD3D11& inst = _gManager.lookup(buffer);

	if(inst._nativeBuffer != 0)
	{

		UINT stride = sizeof(alfar::Vector3);
		UINT offset = 0;
		gD3D11Device->_context->IASetVertexBuffers( 0, 1, &inst._nativeBuffer, &stride, &offset);
	}
}

//---------------

UINT gfxbufferd3d11::GFXToD3D11BindFlags(GFXBufferType type)
{
	switch (type)
	{
	case gfx::VERTEX_BUFFER:
		return D3D11_BIND_VERTEX_BUFFER;
		break;
	case gfx::INDEX_BUFFER:
		return D3D11_BIND_INDEX_BUFFER;
		break;
	case gfx::CONSTANT_BUFFER:
		return D3D11_BIND_CONSTANT_BUFFER;
		break;
	default:
		break;
	}

	assert(0);
	return D3D11_BIND_VERTEX_BUFFER;
}

//------------------

void gfxbufferd3d11::update(ID buffer, void* data, uint32 size)
{
	GFXBufferD3D11& b = _gManager.lookup(buffer);

	gD3D11Device->_context->UpdateSubresource(b._nativeBuffer,0, NULL, data, 0, 0);
}