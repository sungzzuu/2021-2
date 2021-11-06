cbuffer cbPlayerInfo : register(b0)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxWorld : packoffset(c0);
};

cbuffer cbFrameworkInfo : register(b3)
{
	float		gfCurrentTime : packoffset(c0.x);
	float		gfElapsedTime : packoffset(c0.y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_DIFFUSED_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSDiffused(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.color = input.color;

	return(output);
}

float4 PSDiffused(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
	output.color = input.color;

	return(output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTexture : register(t0);

SamplerState gWrapSamplerState : register(s0);
SamplerState gClampSamplerState : register(s1);
SamplerState gMirrorSamplerState : register(s2);

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input, uint primitiveID : SV_PrimitiveID) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);

	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
VS_TEXTURED_OUTPUT VSBillboard(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSBillboard(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.SampleLevel(gWrapSamplerState, input.uv, 0);

	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtFireTextures[2] : register(t1);

float4 PSAnimatedFireWithTexture(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor0 = gtxtFireTextures[0].SampleLevel(gWrapSamplerState, input.uv, 0);
	float4 cColor1 = gtxtFireTextures[1].SampleLevel(gMirrorSamplerState, float2(input.uv.x, input.uv.y + gfCurrentTime), 0);

	float4 cOutput = float4(cColor0.rgb + (cColor0.rgb * cColor1.rgb), cColor0.a);

	return(cOutput);
}

Texture2D gtxtFireAndNoiseTextures[3] : register(t3); //0: Fire Texture, 1: Noise Texture, 2: Noise Texture

float4 PSAnimatedFireWithNoise(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cNoise = gtxtFireAndNoiseTextures[1].SampleLevel(gMirrorSamplerState, float2(input.uv.x, input.uv.y + gfCurrentTime), 0);
	float4 cColor0 = gtxtFireAndNoiseTextures[0].SampleLevel(gWrapSamplerState, float2(input.uv.x, saturate(input.uv.y + cNoise.r)), 0);
	float4 cColor1 = gtxtFireAndNoiseTextures[0].SampleLevel(gWrapSamplerState, input.uv, 0);

	float4 cOutput = float4(cColor1.rgb * 0.5f + cColor1.rgb * cColor0.rgb, cColor1.a);

	return(cOutput);
}

float4 PSAnimatedFireWithMultipleNoise(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor0 = gtxtFireAndNoiseTextures[0].SampleLevel(gWrapSamplerState, input.uv, 0);
	float4 cColor1 = gtxtFireAndNoiseTextures[1].SampleLevel(gMirrorSamplerState, float2(input.uv.x, frac(input.uv.y + gfCurrentTime)), 0);
	float4 cColor2 = gtxtFireAndNoiseTextures[2].SampleLevel(gMirrorSamplerState, float2(input.uv.x, frac(input.uv.y + gfCurrentTime)), 0);
	float4 cColor3 = gtxtFireAndNoiseTextures[0].SampleLevel(gWrapSamplerState, float2(input.uv.x, saturate(input.uv.y + cColor1.r)), 0);
	float4 cColor4 = gtxtFireAndNoiseTextures[0].SampleLevel(gWrapSamplerState, float2(input.uv.x, saturate(input.uv.y + cColor2.r)), 0);

	float4 cOutput = float4(lerp(cColor3.rgb, cColor4.rgb, 0.3f), lerp(cColor0.a, lerp(cColor3.a, cColor4.a, 0.7f), 0.4f));

	return(cOutput);
}

