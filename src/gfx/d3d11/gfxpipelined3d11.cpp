#define INITGUID 

#include "gfx/d3d11/gfxpipelined3d11.h"
#include "gfx/d3d11/gfxbufferd3d11.h"
#include "gfx/d3d11/gfxdeviced3d11.h"

#include "core/logger.h"

#include <D3D11Shader.h>
#include <D3Dcompiler.h>
 #pragma comment(lib, "d3dcompiler.lib")


using namespace gfx;
using namespace gfxdeviced3d11;

ObjectManager<GFXPipelineD3D11, MAX_PIPELINE_OBJECT> gfxpipelined3d11::_gManager;

void gfxpipelined3d11::create()
{
	GFXPipelineD3D11& s = gfxpipelined3d11::_gManager.addAndGet();

	s._nativePixel = 0;
	s._nativeVertex = 0;
}

void gfxpipelined3d11::destroy(ID shader)
{
	GFXPipelineD3D11& s = gfxpipelined3d11::_gManager.lookup(shader);

	if(s._nativePixel)
		s._nativePixel->Release();

	if(s._nativeVertex)
		s._nativeVertex->Release();

	gfxpipelined3d11::_gManager.remove(shader);
}

//******* DEBUG TO REMOVE ? *******************************
//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromSource(const char* source, char* entrypoint, char* shadermodel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
	hr = D3DCompile( source, strlen(source), NULL, NULL, NULL, entrypoint, shadermodel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
			baldr::logger::log( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}

//**************************************************************************************

void gfxpipelined3d11::initFromSource(ID shader, const char* source)
{
	GFXPipelineD3D11& s = gfxpipelined3d11::_gManager.lookup(shader);
	GFXPipeline& os = gfxpipeline::_gManager.lookup(shader);

	HRESULT hr;

	strcpy_s(s.source, source);

	ID3DBlob* pVSBlob = NULL;

	if(strstr(source, "main_vs"))
	{
		hr = CompileShaderFromSource( source, "main_vs", "vs_4_0", &pVSBlob );
		if( FAILED( hr ) )
		{
			baldr::logger::log("Error compiling the shader source for vertex");
		}
		else
		{
			// Create the vertex shader
			hr = gD3D11Device->_device->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &s._nativeVertex );
			if( FAILED( hr ) )
			{	
				baldr::logger::log("Error creating the vertex shader %s", source);
				pVSBlob->Release();
			}
			else
			{
				 // Define the input layout
				D3D11_INPUT_ELEMENT_DESC layout[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};
				UINT numElements = ARRAYSIZE( layout );

				// Create the input layout
				hr = gD3D11Device->_device->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
					pVSBlob->GetBufferSize(), &s._vertexLayout );

				extractConstants(shader, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize());

				pVSBlob->Release();
				if( FAILED( hr ) )
				{
					baldr::logger::log("couldn't create Input layout");	
				}
				else
				{
					os._typecontains |= gfx::VERTEX_SHADER;
				}
			}
		}

	}

	if(strstr(source, "main_ps"))
	{
		hr = CompileShaderFromSource( source, "main_ps", "ps_4_0", &pVSBlob );
		if( FAILED( hr ) )
		{
			baldr::logger::log("Error compiling the shader source for pixel for %s", source);
		}
		else
		{
			// Create the pixel shader
			hr = gD3D11Device->_device->CreatePixelShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &s._nativePixel );

			extractConstants(shader, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize());

			pVSBlob->Release();

			if( FAILED( hr ) )
			{	
				baldr::logger::log("Error creating the pixel shader");
			}
			else
			{
				os._typecontains |= gfx::PIXEL_SHADER;
			}
		}
	}
}

void gfx::gfxpipelined3d11::extractConstants(ID pipeline, LPVOID buffer, SIZE_T size)
{
	HRESULT hr;
	ID3D11ShaderReflection* reflection = 0;
	GFXPipelineD3D11& s = gfxpipelined3d11::_gManager.lookup(pipeline);
	GFXPipeline& os = gfxpipeline::_gManager.lookup(pipeline);

	hr = D3DReflect(buffer, size, IID_ID3D11ShaderReflection, (void**)&reflection);

	D3D11_SHADER_DESC desc;
	reflection->GetDesc(&desc);

	for(uint32 i = 0; i < desc.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* cb = reflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		cb->GetDesc(&bufferDesc);

		if(i <= 1)
		{
			if(i == 0 && strcmp(bufferDesc.Name, "CameraInfo") != 0)
			{
				baldr::logger::log("The shader define qnother thing than the CameraInfo in b0 : %s", s.source);
			}

			if(i == 1 && strcmp(bufferDesc.Name, "ObjectInfo") != 0)
			{
				baldr::logger::log("The shader define qnother thing than the ObjectInfo in b1 : %s", s.source);
			}
		}
		else
		{
			for(uint32 j = 0; j < bufferDesc.Variables; ++j)
			{
				ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC varDesc;

				var->GetDesc(&varDesc);
			
				uint32 maxVal = 0;
				for(uint32 i = 0; i < MAX_PIPELINE_CONSTANTES; ++i)
				{
					if(os._variables[i].size == 0)
					{//we found the end of the array
						maxVal = 0;
						break;
					}

					if(strcmp(os._variables[i].name, varDesc.Name) == 0)
					{//this param already exist in the array, no need to record it AGAIN
						maxVal = -1;
						break;
					}
				}

				if(maxVal >= 0)
				{
					strcpy_s(os._variables[maxVal].name, 256, varDesc.Name);
					os._variables[maxVal].size = varDesc.Size;
					os._variables[maxVal].offset = maxVal == 0 ? 0 : os._variables[maxVal-1].offset + os._variables[maxVal-1].size;
				}
			}
		}
	}
}

void gfx::gfxpipelined3d11::bind(ID shader)
{
	GFXPipelineD3D11& s = _gManager.lookup(shader);

	if(s._nativeVertex != 0)
	{
		gD3D11Device->_context->IASetInputLayout( s._vertexLayout );
		gD3D11Device->_context->VSSetShader(s._nativeVertex, NULL, 0);
	}

	if(s._nativePixel != 0)
	{
		gD3D11Device->_context->PSSetShader(s._nativePixel, NULL, 0);
	}
}

void gfx::gfxpipelined3d11::uploadBuffer(ID buffer, uint8 slot)
{
	GFXBufferD3D11& b = gfxbufferd3d11::_gManager.lookup(buffer);

	gD3D11Device->_context->VSSetConstantBuffers(slot, 1, &b._nativeBuffer);
}