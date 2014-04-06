cbuffer CameraInfo : register( b0 )
{
	matrix View;
	matrix Projection;
}

cbuffer ObjectInfo : register( b1)
{
	matrix World;
}


float4 main_vs( float4 Pos : POSITION ) : SV_POSITION
{
    float4 p = mul( Pos, World );
    p = mul( p, View );
    p = mul( p, Projection );
    return p;
}

float4 main_ps( float4 Pos : SV_POSITION ) : SV_Target
{
    return float4( 1.0f, 1.0f, 0.0f, 1.0f );    // Yellow, with Alpha = 1
}