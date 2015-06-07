float4x4 World;
float4x4 View;
float4x4 Projection;
Texture2D InputTexture;

int HundredsPlace = 1;
int TensPlace = 0;
int OnesPlace = 0;

float Opacity = 0.75;

int StationType = 0;

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

float4 PixelShaderFunction(VertexShaderOutput input) : SV_Target
{
// 
	float4 output;
	if(input.TextureUV.x < 0.25)
		output = InputTexture.Sample(LinearSampler, float2((12 - HundredsPlace)*0.0625 +input.TextureUV.x/4, input.TextureUV.y));
			
	else if (input.TextureUV.x < 0.5)
		output = InputTexture.Sample(LinearSampler, float2(TensPlace*0.0625 +(input.TextureUV.x-0.25)/4, input.TextureUV.y));
		
	else if (input.TextureUV.x < 0.75)
		output = InputTexture.Sample(LinearSampler, float2(OnesPlace*0.0625 +(input.TextureUV.x-0.5)/4, input.TextureUV.y));
		
	else if (TensPlace > 0 || OnesPlace > 0 || HundredsPlace > 0)
		output = InputTexture.Sample(LinearSampler, float2(13*0.0625 +(input.TextureUV.x-0.75)/4, input.TextureUV.y)); // % on
	else
		output = InputTexture.Sample(LinearSampler, float2(14*0.0625 +(input.TextureUV.x-0.75)/4, input.TextureUV.y)); // % off
	
	//output = InputTexture.Sample(LinearSampler, input.TextureUV);
    return output * GetHudColor(StationType);;
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