#include "gfx/d3d11/gfxdeviced3d11.h"
#include "core/logger.h"

using namespace gfx;

GFXDeviceD3D11* gfx::gD3D11Device = NULL;


//=================================================================


uint8_t gfxdeviced3d11::create(void* pWindow)
{
	GFXDeviceD3D11* dev = new GFXDeviceD3D11();
	gDevice = dev;
	gD3D11Device = dev;


	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect( (HWND)pWindow, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)pWindow;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

	
    hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &dev->_swapchain, &dev->_device, NULL, &dev->_context );

    if( FAILED( hr ) )
	{
		
		baldr::logger::log("Creation of device failed");
		return FALSE;
	}

	    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
	hr = dev->_swapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
	{
		baldr::logger::log("Couldn't get buffer from D3D11 device");
        return FALSE;
	}

	hr = dev->_device->CreateRenderTargetView( pBackBuffer, NULL, &dev->_defaultColorbuffer );
    pBackBuffer->Release();
    if( FAILED( hr ) )
	{
        return FALSE;
	}

	dev->_context->OMSetRenderTargets( 1, &dev->_defaultColorbuffer, NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

	dev->_context->RSSetViewports( 1, &vp );

	return TRUE;
}

//=========================================

void gfxdeviced3d11::destroy()
{
	if(!gD3D11Device)
		return;

	gD3D11Device->_defaultColorbuffer->Release();

	gD3D11Device->_swapchain->Release();
	gD3D11Device->_context->Release();
	gD3D11Device->_device->Release();
}

//=========================================

void gfxdeviced3d11::swap()
{
	gD3D11Device->_swapchain->Present(0,0);
}

//========================================

void gfxdeviced3d11::clear(alfar::Vector4 pClearColor)
{
	gD3D11Device->_context->ClearRenderTargetView(gD3D11Device->_defaultColorbuffer, &pClearColor.x);
}

//========================================

void gfxdeviced3d11::draw(GFXTopology type, uint32 number, uint32 offset)
{
	gD3D11Device->_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	gD3D11Device->_context->Draw(number, offset);
}