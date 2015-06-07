float4x4 World;
float4x4 View;
float4x4 Projection;
Texture2D Texture;
Texture2D BloomEffectTex;  

//Bloom effect Variables
float mag = 0.008;
float alpha;

//Operate on a high range (0.5 - 1.0) or the full range (0.0 - 1.0)
bool hirange = true;

const float2 offsets[12] = {
   -0.326212, -0.405805,
   -0.840144, -0.073580,
   -0.695914,  0.457137,
   -0.203345,  0.620716,
    0.962340, -0.194983,
    0.473434, -0.480026,
    0.519456,  0.767022,
    0.185461, -0.893124,
    0.507431,  0.064425,
    0.896420,  0.412458,
   -0.321940, -0.932615,
   -0.791559, -0.597705,
};

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

         

SamplerState BloomSampler
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
    return Texture.Sample(LinearSampler, input.TextureUV);
}

float4 ShipBloomEffect(VertexShaderOutput Input) : SV_Target
{ 
    float4 sum = BloomEffectTex.Sample(BloomSampler, Input.TextureUV);
    float4 tex;
    
    //accumulate the color values from 12 neighboring pixels
    for(int i = 0; i < 12; i++){
        tex = BloomEffectTex.Sample(BloomSampler, Input.TextureUV + mag * offsets[i]);
        
        sum += tex;
    }
    //average the sum
    sum /= 13;
    
    //fix alpha
    sum.a = alpha;
    
    //expand the higher range if applicable
    if (hirange) {
        sum.r = (sum.r - 0.5) * 2.0;
        sum.g = (sum.g - 0.5) * 2.0;
        sum.b = (sum.b - 0.5) * 2.0;
    }
    
    return sum;
}

technique10 Technique1
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VertexShaderFunction() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PixelShaderFunction() ) );
    }
    
    pass P1
    {
		SetVertexShader( CompileShader( vs_4_0, VertexShaderFunction() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, ShipBloomEffect() ) );
    }
}