float4x4 World;
float4x4 View;
float4x4 Projection;

float NoiseX = 0;
float NoiseY = 0;
float Radius = 1.0f;

float Opacity = 1.0f;
int ShieldType = 5;

float PI = 3.14159265;

Texture2D InputTexture;
Texture2D NoiseTexture;
Texture2D ShieldMask;

struct VertexShaderInput
{
	float4 Position		: POSITION;
	float4 Color 		: COLOR0;
	float2 TextureUV	: TEXCOORD0;
};

float4 GetHudColor(int shieldType)
{
	float4 outColor = float4(1,1,1,Opacity);
	
	if(shieldType == 0)
	{
		// red - swivel (196, 72, 55)
		outColor = float4(0.768627,0.282352,0.215686,Opacity);
	}
	if(shieldType == 1)
	{
		// green - blitz (97, 150, 59)
		outColor = float4(0.380392,0.588235,0.250980,Opacity);
	}
	if(shieldType == 2)
	{
		// yellow - blaze (212, 190, 59)
		outColor = float4(0.831372,0.745098,0.231372,Opacity);
	}
	if(shieldType == 3)
	{
		// blue - heavy (84, 138, 197)
		outColor = float4(0.329411,0.541176,0.772549,Opacity);
	}
	if(shieldType == 5)
	{
		// white - sheild
		outColor = float4(1,1,1,Opacity);
	}
	
	return outColor;
}

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
	float4 noiseCoord = NoiseTexture.Sample(ClampSampler, float2(NoiseX + (input.TextureUV.x / 400) , input.TextureUV.y));
	float4 shieldMask = ShieldMask.Sample(ClampSampler, input.TextureUV);
	float4 shieldSample = InputTexture.Sample(ClampSampler, input.TextureUV);
	float4 red = float4(1, 0, 0, 1);
	
	//return shieldSample;
	
	noiseCoord -= 0.5;
	
	float2 texCoord;
	
	//check to see if the point is beyond the exterior of the shield
	
	if(shieldMask.r == red.r && shieldMask.g == red.g == shieldMask.b == red.b && shieldMask.a == red.a)
	{
		texCoord.x = input.TextureUV.x;
		
	}else{
		texCoord.x = input.TextureUV.x + noiseCoord.x;
	}
	
	texCoord.y = input.TextureUV.y;
	
    float4 sampleColor = shieldSample + InputTexture.Sample(ClampSampler, texCoord);
	sampleColor /= 2;

    return sampleColor * GetHudColor(ShieldType);
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