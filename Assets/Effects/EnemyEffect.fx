float4x4 World;
float4x4 View;
float4x4 Projection;
Texture2D InputTexture;
int IsHit;

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
	
	VertexShaderOutput output;
	output.Position = mul(input.Position, wvp);
	output.TextureUV = input.TextureUV;
	output.Color = input.Color;
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : SV_Target
{
	float4 color;
	
	if(IsHit > 0)
	{
		if(input.Color.w > 0)
		{
			color = InputTexture.Sample(LinearSampler, input.TextureUV) + float4(1, 0, 0, 0.0f);
		}
		else
		{
			color = InputTexture.Sample(LinearSampler, input.TextureUV);
		}
	}else{
		color = InputTexture.Sample(LinearSampler, input.TextureUV);
	}
	
    return color;
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