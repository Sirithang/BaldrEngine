#include "core/engine.h"
#include "core/logger.h"

#include "gfx/gfxpipeline.h"
#include "gfx/gfxbuffer.h"

#include "vector4.h"
#include "vector3.h"

#include "mat4x4.h"

using namespace baldr;
using namespace baldr::engine;
using namespace gfx;

Engine engine::gEngine;

ID shader;
ID buffer;
ID bufferCam;
ID bufferWorld;

struct ObjectInfo
{
	alfar::Matrix4x4 World;
} oinfo;

uint8_t engine::init(WinHandle* window)
{
	logger::logFunc = loggerimpl::consoleLog;

	gfx::setupD3D11Interface();

	if(gfxdevice::create(window->nativeHandle) == FALSE)
	{
		logger::log("Error initializing gfxdevice. Check Log");
		return FALSE;
	}

	logger::log("Engine initialized");

	//---- test code ------

	shader = gfxpipeline::create();

	FILE* f = fopen("data/test.shader", "rb");
	if(f)
	{
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char *string = (char*)malloc(fsize + 1);
		fread(string, fsize, 1, f);
		fclose(f);

		string[fsize] = 0;

		gfxpipeline::initFromSource(shader, string);

		free(string);
	}
	else
	{
		logger::log("Couldn't open shader");
	}

	alfar::Vector3 datas[] = { alfar::vector3::create(0.0f, 0.5f, 0.5f), alfar::vector3::create(0.5f, -0.5f, 0.5f), alfar::vector3::create(-0.5f, -0.5f, 0.5f) };

	buffer = gfx::gfxbuffer::create(GFXBufferType::VERTEX_BUFFER, GFXBufferUsage::IMMUTABLE, &datas[0].x, 3*sizeof(alfar::Vector3));

	struct CamInfo
	{
		alfar::Matrix4x4 View;
		alfar::Matrix4x4 Projection;
	} cinfo;

	cinfo.Projection = alfar::mat4x4::identity();
	cinfo.View = alfar::mat4x4::identity();

	

	oinfo.World = alfar::mat4x4::identity();//alfar::mat4x4::translation(alfar::vector3::create(0.4f, 0, 0));

	bufferCam = gfxbuffer::create(GFXBufferType::CONSTANT_BUFFER, GFXBufferUsage::DEFAULT,&cinfo, sizeof(CamInfo)); 
	bufferWorld = gfxbuffer::create(GFXBufferType::CONSTANT_BUFFER, GFXBufferUsage::DEFAULT, &oinfo, sizeof(ObjectInfo));

	return TRUE;
}

//-----------------------

float translation = 0;
void engine::update()
{
	translation += 0.00001f;

	gfxdevice::clear(alfar::vector4::create(1.0f, 0.0F, 1.0f, 1.0f));

	oinfo.World = alfar::mat4x4::translation(alfar::vector3::create(translation, 0, 0));

	gfxbuffer::update(bufferWorld, &oinfo, sizeof(ObjectInfo));

	gfxpipeline::bind(shader);

	gfxpipeline::uploadBuffer(bufferCam, 0);
	gfxpipeline::uploadBuffer(bufferWorld, 1);

	gfxbuffer::bind(buffer);
	gfxdevice::draw(GFXTopology::TOPOLGY_TRIANGLELIST, 3, 0);

	gfxdevice::swap();
}

//-----------------------

void engine::shutdown()
{
	gfx::gfxdevice::destroy();
}