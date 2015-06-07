float4x4 World;
float4x4 View;
float4x4 Projection;

//Texture layers
Texture2D BaseTexture;
Texture2D GroundTexture;
Texture2D WaterTexture;
Texture2D AtmoTexture;
Texture2D CloudTexture;

//Colors for the texture layers (randomized at object initialization)
float4 BaseColor;
float4 GroundColor;
float4 WaterColor;
float4 AtmoColor;
float4 CloudColor;

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

SamplerState ClampSampler
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
	float4 color = float4(0, 0, 0, 0);
	
    //Sample and color the base layer
    float4 tempColor = BaseTexture.Sample(LinearSampler, input.TextureUV) * BaseColor;
    if(tempColor.w > 0)
		color += tempColor;
		
    //Sample and color the ground layer
    tempColor = GroundTexture.Sample(LinearSampler, input.TextureUV);
	if(tempColor.w > 0)
		color += tempColor;
		
    //Sample and color the water layer
    tempColor = WaterTexture.Sample(LinearSampler, input.TextureUV);
    if(tempColor.w > 0)
		color += tempColor;
		
    //Sample the atmo layer
    //tempColor = AtmoTexture.Sample(LinearSampler, input.TextureUV);
    //if(tempColor.w > 0)
	//	color += tempColor;
		
    //add the clouds
    tempColor = CloudTexture.Sample(LinearSampler, input.TextureUV);
	if(tempColor.w > 0)
		color += tempColor;
    
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