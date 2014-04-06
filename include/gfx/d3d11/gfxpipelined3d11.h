#ifndef _BALDER_GFXSHADERD3D11_H_
#define _BALDER_GFXSHADERD3D11_H_

#ifdef D3D11RENDERER

#include"gfx/gfxpipeline.h"

#include <windows.h>
#include <d3d11.h>

namespace gfx
{
	struct GFXPipelineD3D11 : public ManagedObject
	{
		ID3D11VertexShader* _nativeVertex;
		ID3D11PixelShader* _nativePixel;
		ID3D11InputLayout* _vertexLayout;

		char source[1024];
	};


	namespace gfxpipelined3d11
	{
		extern ObjectManager<GFXPipelineD3D11, MAX_PIPELINE_OBJECT> _gManager;

		void create();
		void destroy(ID shader);

		void initFromSource(ID shader, const char* source);
		void extractConstants(ID pipeline, LPVOID buffer, SIZE_T size);

		void uploadBuffer(ID buffer, uint8 slot);

		void bind(ID shader);
	}
}

#endif

#endif