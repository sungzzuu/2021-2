cbuffer cbGameObjectInfo : register(b0)
{
	matrix		gmtxWorld : packoffset(c0);
	uint		gnMaterial : packoffset(c4);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
	float3		gvCameraPosition : packoffset(c8);
};

#include "Light.hlsl"

struct CB_TOOBJECTSPACE
{
	matrix		mtxToTexture;
	float4		f4Position;
};

cbuffer cbProjectorSpace : register(b5)
{
	CB_TOOBJECTSPACE gcbToProjectorSpaces[MAX_LIGHTS];
};

cbuffer cbToLightSpace : register(b6)
{
	CB_TOOBJECTSPACE gcbToLightSpaces[MAX_LIGHTS];
};

//ConstantBuffer<CB_TOOBJECTSPACE> gcbToLightSpaces[MAX_LIGHTS] : register(b6);

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

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.color = input.color;

	return(output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
};

VS_LIGHTING_OUTPUT VSLighting(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output;

	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxWorld);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	return(output);
}

float4 PSLighting(VS_LIGHTING_OUTPUT input) : SV_TARGET
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);

//	return(cIllumination);
	return(float4(input.normalW * 0.5f + 0.5f, 1.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_CIRCULAR_SHADOW_INPUT
{
	float3 center : POSITION;
	float2 size : TEXCOORD;
};

VS_CIRCULAR_SHADOW_INPUT VSCircularShadow(VS_CIRCULAR_SHADOW_INPUT input)
{
	return(input);
}

struct GS_CIRCULAR_SHADOW_GEOMETRY_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

static float2 pf2UVs[4] = { float2(0.0f,1.0f), float2(0.0f,0.0f), float2(1.0f,1.0f), float2(1.0f,0.0f)};

[maxvertexcount(4)]
void GSCircularShadow(point VS_CIRCULAR_SHADOW_INPUT input[1], inout TriangleStream<GS_CIRCULAR_SHADOW_GEOMETRY_OUTPUT> outStream)
{
	float fHalfWidth = input[0].size.x * 0.5f;
	float fHalfDepth = input[0].size.y * 0.5f;

	float3 f3Right = float3(1.0f, 0.0f, 0.0f);
	float3 f3Look = float3(0.0f, 0.0f, 1.0f);

	float4 pf4Vertices[4];
	pf4Vertices[0] = float4(input[0].center.xyz - (fHalfWidth * f3Right)- (fHalfDepth * f3Look), 1.0f);
	pf4Vertices[1] = float4(input[0].center.xyz - (fHalfWidth * f3Right) + (fHalfDepth * f3Look), 1.0f);
	pf4Vertices[2] = float4(input[0].center.xyz + (fHalfWidth * f3Right)- (fHalfDepth * f3Look), 1.0f);
	pf4Vertices[3] = float4(input[0].center.xyz + (fHalfWidth * f3Right) + (fHalfDepth * f3Look), 1.0f);

	GS_CIRCULAR_SHADOW_GEOMETRY_OUTPUT output;
	for (int i = 0; i < 4; i++)
	{
		output.position = mul(mul(pf4Vertices[i], gmtxView), gmtxProjection);
		output.uv = pf2UVs[i];

		outStream.Append(output);
	}
}

Texture2D gtxtCircularShadowTexture : register(t0); 

SamplerState gssWrap : register(s0);

float4 PSCircularShadow(GS_CIRCULAR_SHADOW_GEOMETRY_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtCircularShadowTexture.Sample(gssWrap, input.uv);
	cColor.rgb = float3(1.0f, 1.0f, 1.0f) - cColor.rgb;
	cColor.a = cColor.r;

	return(cColor);
}
