float4x4 World;
float4x4 View;
float4x4 Projection;

Texture2D InputTexture;
float2 ImageSize;
float2 TextureSize;
float2 ImageScale;
float Frame = 0;

struct VertexShaderInput
{
	float4 Position		: POSITION;
	float4 Color 		: COLOR0;
	float2 TextureUV	: TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 Position		: SV_POSITION;
	float4 Color 		: COLOR0;
	float2 TextureUV	: TEXCOORD0;
};

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	float4x4 wvp = mul( mul( World, View ) , Projection );
	float width = floor(TextureSize.x / ImageSize.x);
	float2 scale = ImageSize / TextureSize;
	float2 offset = float2(Frame % width, floor(Frame / width));
	
	VertexShaderOutput output;
	output.Position = input.Position;
	output.Position.x = output.Position.x * ImageScale.x;
	output.Position.z = output.Position.z * ImageScale.y;
	output.Position = mul(output.Position, wvp);
	output.TextureUV = (offset + input.TextureUV) * scale;
	output.Color = input.Color;
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : SV_Target
{
    return InputTexture.Sample(LinearSampler, input.TextureUV);
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