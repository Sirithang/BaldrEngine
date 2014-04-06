SHADERS
=======

Shaders in the baldre engine are abstracted in a single file. In the code, a set of shaders/params is called a GFXPipeline.

A Gfx pipeline is just a set of shader for each stage (GS/VS/PS etc...) + information of the constant buffer linked to them.

Constant buffer in the engine MUST follow that convention (ATM it's manual, futur plan will include a shader preprocessing to add that auto)


b0 should contain the camera info cbuffer:

cbuffer CameraInfo : register( b0 )
{
	matrix View;
	matrix Projection;
}

b1 contain the object info cbuffer :

cbuffer ObjectInfo : register( b1 )
{
	matrix World;
}

b2 contain the user defined parameter (it will be recorded in GFX pipeline & used by material to autoset things)
