float4x4 World;
float4x4 View;
float4x4 Projection;

int StationType;
float StationHealth;

Texture2D InputTexture;
Texture2D GradientTexture;
float Opacity = 0.5;

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
	AddressU = Clamp;
	AddressV = Clamp;
};

float4 GetHudColor(int stationType)
{
	float4 outColor = float4(1,1,1,Opacity);
	
	if(stationType == 0)
	{
		// red - swivel (196, 72, 55)
		outColor = float4(0.768627,0.282352,0.215686,Opacity);
	}
	if(stationType == 1)
	{
		// green - blitz (97, 150, 59)
		outColor = float4(0.380392,0.588235,0.250980,Opacity);
	}
	if(stationType == 2)
	{
		// yellow - blaze (212, 190, 59)
		outColor = float4(0.831372,0.745098,0.231372,Opacity);
	}
	if(stationType == 3)
	{
		// blue - heavy (84, 138, 197)
		outColor = float4(0.329411,0.541176,0.772549,Opacity);
	}
	if(stationType == 4)
	{
		// grey - engineering
		outColor = float4(0.75,0.75,0.75,Opacity);
	}
	if(stationType == 5)
	{
		// white - sheild
		outColor = float4(1,1,1,Opacity);
	}
	if(stationType == 6)
	{
		// cyan - pilot?
		outColor = float4(0,1,1,Opacity);
	}
	
	return outColor;
}
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	float4x4 wvp = mul( mul( World, View ) , Projection );
	
	VertexShaderOutput output;
	output.Position = mul(input.Position, wvp);
	output.TextureUV = input.TextureUV;
	output.Color = input.Color;
	return output;
}

float4 AlphaPixel(VertexShaderOutput input) : SV_Target
{
	float2 texCoord = float2(input.TextureUV.x, input.TextureUV.y);
	float4 gradientCoord = GradientTexture.Sample(LinearSampler, texCoord);
	
	if(gradientCoord.r <= (StationHealth / 100.0))
		return InputTexture.Sample(LinearSampler, input.TextureUV) * GetHudColor(StationType);
	
    return float4(0, 0, 0, 0);
}

float4 StandardPixel(VertexShaderOutput input) : SV_Target
{
	return InputTexture.Sample(LinearSampler, input.TextureUV) * GetHudColor(StationType);
}
technique10 AlphaHud
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VertexShaderFunction() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, AlphaPixel() ) );
    }
}

technique10 StandardHud
{
	pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VertexShaderFunction() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, StandardPixel() ) );
    }
}

