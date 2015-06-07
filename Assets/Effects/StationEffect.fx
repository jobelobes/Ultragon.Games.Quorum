#define PI 3.14159265

float4x4 World;
float4x4 View;
float4x4 Projection;

float ElapsedTime;
float PhaseSpeed;
float Amplitude;

int StationType;
int StationHealth;

float4 ActiveColor;
Texture2D SegmentTexture;
Texture2D StationTexture;
Texture2D ActiveTexture;

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
	//float frequency = 1 / ElapsedTime;
	//float frequency = PhaseSpeed / 20;
	//float waveNumber = (2 * PI) / 20;
	//float angularVelocity = 2 * PI / ElapsedTime;
	
	// I think I just need to change this line to get the station health to go?
	//float4 pulseColor = ActiveColor *  sin((waveNumber * activeTex.w) - angularVelocity);
	int health = 8 - trunc((StationHealth * 8) / 100.0);
	
	float4 segmentTex = SegmentTexture.Sample(LinearSampler, float2(health * 0.125 + input.TextureUV.x / 8, input.TextureUV.y));
	float4 activeTex = ActiveTexture.Sample(LinearSampler, input.TextureUV);
	
	float4 color = float4(segmentTex.xyz + activeTex.w * ActiveColor.w * segmentTex.xyz * ActiveColor.xyz * 1.25f, segmentTex.w);
	
	if(color.w == 0 || StationType == 0)
		color = StationTexture.Sample(LinearSampler, input.TextureUV);
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