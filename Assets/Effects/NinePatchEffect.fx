float4x4 World;
float4x4 View;
float4x4 Projection;
Texture2D InputTexture;
float2 Size;
float2 TextureSize;

struct VertexShaderInput
{
	float4 Position		: POSITION;
	float2 TextureUV	: TEXCOORD0;
	uint Index 			: COLOR0;
};

struct VertexShaderOutput
{
	float4 Position		: SV_POSITION;
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
	
	VertexShaderOutput output;
	float y = ceil(input.Index / 4.0);
	int x = fmod(input.Index, 4);
	
	if(x == 3.0)
		input.Position.x = Size.x - TextureSize.x / 3.0;
	else if(x == 2.0)
		input.Position.x = TextureSize.x / 3.0;
	else if(x == 1.0)
		input.Position.x = 0;
	else
		input.Position.x = Size.x;

	if(y > 3)
		input.Position.z = 0;
	else if(y > 2)
		input.Position.z = TextureSize.y / 3.0;
	else if(y > 1.1)
		input.Position.z = Size.y - TextureSize.y / 3.0;
	else
		input.Position.z = Size.y;
	
	output.Position = mul(input.Position, wvp);
	output.TextureUV = input.TextureUV;
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