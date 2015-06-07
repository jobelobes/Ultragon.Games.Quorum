float4x4 World;
float4x4 View;
float4x4 Projection;
float4 Color;

struct VertexShaderInput
{
	float4 Position	: POSITION;
	float4 Color 	: COLOR0;
};

struct VertexShaderOutput
{
	float4 Position	: SV_POSITION;
	float4 Color 	: COLOR0;
};


VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	float4x4 wvp = mul( mul( World, View ) , Projection );
	
	VertexShaderOutput output;
	output.Position = mul(input.Position, wvp);
	output.Color = input.Color;
	return output; 
} 

float4 PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
    return Color;
}
 
technique10 Technique1
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VertexShaderFunction() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PixelShaderFunction() ) );
    }
}