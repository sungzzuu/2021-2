cbuffer cbPlayerInfo : register(b0)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
	float3		gvCameraPosition : packoffset(c8);

};

struct MATERIAL
{
	float4				m_cAmbient;
	float4				m_cDiffuse;
	float4				m_cSpecular; //a = power
	float4				m_cEmissive;

	matrix				gmtxTexture;
	int2				gi2TextureTiling;
	float2				gf2TextureOffset;
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxGameObject : packoffset(c0);
	MATERIAL	gMaterial : packoffset(c4);

};

cbuffer cbFrameworkInfo : register(b3)
{
	float 		gfCurrentTime;
	float		gfElapsedTime;
	float2		gf2CursorPos;
};

cbuffer cbWaterInfo : register(b4)
{
	matrix		gf4x4TextureAnimation : packoffset(c0);
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
SamplerState gSamplerState : register(s0);
SamplerState gClampSamplerState : register(s1);

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

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

VS_TEXTURED_OUTPUT VSSpriteAnimation(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = mul(float3(input.uv, 1.0f), (float3x3)(gMaterial.gmtxTexture)).xy;

	return(output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);

	return(cColor);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//



Texture2D<float4> gtxtTerrainBaseTexture : register(t1);
Texture2D<float4> gtxtTerrainDetailTextures[3] : register(t2); //t2, t3, t4
Texture2D<float> gtxtTerrainAlphaTexture : register(t5);
//Texture2D<float4> gtxtTerrainAlphaTexture : register(t5);

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.color = input.color;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	return(output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv0);
	float fAlpha = gtxtTerrainAlphaTexture.Sample(gSamplerState, input.uv0);
	//float fAlpha = gtxtTerrainAlphaTexture.Sample(gSamplerState, input.uv0).w;

	float4 cDetailTexColors[3];
	cDetailTexColors[0] = gtxtTerrainDetailTextures[0].Sample(gSamplerState, input.uv1 * 1.0f);
	cDetailTexColors[1] = gtxtTerrainDetailTextures[1].Sample(gSamplerState, input.uv1 * 0.5f);
	cDetailTexColors[2] = gtxtTerrainDetailTextures[2].Sample(gSamplerState, input.uv1);

	float4 cColor = cBaseTexColor * cDetailTexColors[0];
	cColor += lerp(cDetailTexColors[1] * 0.25f, cDetailTexColors[2], 1.0f - fAlpha);

	cColor = lerp(cDetailTexColors[0], cDetailTexColors[2], 1.0f - fAlpha) ;
	cColor = lerp(cBaseTexColor, cColor, 0.3f) + cDetailTexColors[1] * (1.0f - fAlpha);

	if (fAlpha < 0.5f) cColor = lerp(cDetailTexColors[1], cDetailTexColors[2], 1.0f - fAlpha);
	else if (fAlpha > 0.9f) cColor = lerp(cDetailTexColors[0], cDetailTexColors[1], 1.0f - fAlpha);
	else cColor = lerp(cDetailTexColors[0], cDetailTexColors[1]*0.7f, 1.0f - fAlpha);

	return(cColor);
}

struct VS_WATER_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_WATER_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

VS_WATER_OUTPUT VSTerrainWater(VS_WATER_INPUT input)
{
	VS_WATER_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

Texture2D<float4> gtxtWaterBaseTexture : register(t6);
Texture2D<float4> gtxtWaterDetail0Texture : register(t7);
Texture2D<float4> gtxtWaterDetail1Texture : register(t8);

static matrix<float, 3, 3> sf3x3TextureAnimation = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

#define _WITH_TEXTURE_ANIMATION

//#define _WITH_BASE_TEXTURE_ONLY
#define _WITH_FULL_TEXTURES

#ifndef _WITH_TEXTURE_ANIMATION
float4 PSTerrainWater(VS_WATER_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtWaterBaseTexture.Sample(gSamplerState, input.uv);
	float4 cDetail0TexColor = gtxtWaterDetail0Texture.Sample(gSamplerState, input.uv * 20.0f);
	float4 cDetail1TexColor = gtxtWaterDetail1Texture.Sample(gSamplerState, input.uv * 20.0f);

	float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
#ifdef _WITH_BASE_TEXTURE_ONLY
	cColor = cBaseTexColor;
#else
#ifdef _WITH_FULL_TEXTURES
	cColor = lerp(cBaseTexColor * cDetail0TexColor, cDetail1TexColor.r * 0.5f, 0.35f);
#else
	cColor = cBaseTexColor * cDetail0TexColor;
#endif
#endif

	return(cColor);
}
#else
#define _WITH_CONSTANT_BUFFER_MATRIX
//#define _WITH_STATIC_MATRIX

float4 PSTerrainWater(VS_WATER_OUTPUT input) : SV_TARGET
{
	float2 uv = input.uv;

#ifdef _WITH_STATIC_MATRIX
	sf3x3TextureAnimation._m21 = gfCurrentTime * 0.00125f;
	uv = mul(float3(input.uv, 1.0f), sf3x3TextureAnimation).xy;
#else
#ifdef _WITH_CONSTANT_BUFFER_MATRIX
	uv = mul(float3(input.uv, 1.0f), (float3x3)gf4x4TextureAnimation).xy;
	//	uv = mul(float4(uv, 1.0f, 0.0f), gf4x4TextureAnimation).xy;
#else
	uv.y += gfCurrentTime * 0.00125f;
#endif
#endif

	float4 cBaseTexColor = gtxtWaterBaseTexture.SampleLevel(gSamplerState, uv, 0);
	float4 cDetail0TexColor = gtxtWaterDetail0Texture.SampleLevel(gSamplerState, uv * 20.0f, 0);
	float4 cDetail1TexColor = gtxtWaterDetail1Texture.SampleLevel(gSamplerState, uv * 20.0f, 0);

	float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
#ifdef _WITH_BASE_TEXTURE_ONLY
	cColor = cBaseTexColor;
#else
#ifdef _WITH_FULL_TEXTURES
	cColor = lerp(cBaseTexColor * cDetail0TexColor, cDetail1TexColor.r * 0.5f, 0.35f);
#else
	cColor = cBaseTexColor * cDetail0TexColor;
#endif
#endif

	return(cColor);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture2D gtxtSkyBox : register(t9);

float4 PSSkyBox(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtSkyBox.Sample(gClampSamplerState, input.uv);

	return(cColor);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define _WITH_BILLBOARD_ANIMATION

struct VS_BILLBOARD_INSTANCING_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 instancePosition : INSTANCEPOSITION;
	float4 billboardInfo : BILLBOARDINFO; //(cx, cy, type, texture)
};

struct VS_BILLBOARD_INSTANCING_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	int textureID : TEXTUREID;
};

#define _WITH_BILLBOARD_ANIMATION

VS_BILLBOARD_INSTANCING_OUTPUT VSBillboardInstancing(VS_BILLBOARD_INSTANCING_INPUT input)
{
	VS_BILLBOARD_INSTANCING_OUTPUT output;

	input.position.x *= (input.billboardInfo.x * 0.5f);
	input.position.y *= (input.billboardInfo.y * 0.5f);

	float3 f3Look = normalize(gvCameraPosition - input.instancePosition);
	float3 f3Up = float3(0.0f, 1.0f, 0.0f);
	float3 f3Right = normalize(cross(f3Up, f3Look));

	matrix mtxWorld;
	mtxWorld[0] = float4(f3Right, 0.0f);
	mtxWorld[1] = float4(f3Up, 0.0f);
	mtxWorld[2] = float4(f3Look, 0.0f);
	mtxWorld[3] = float4(input.instancePosition, 1.0f);

	output.position = mul(mul(mul(float4(input.position, 1.0f), mtxWorld), gmtxView), gmtxProjection);

#ifdef _WITH_BILLBOARD_ANIMATION
	if (input.uv.y < 0.7f)
	{
		float fShift = 0.0f;
		uint nResidual = ((uint)gfCurrentTime % 4);
		if (nResidual == 1) fShift = -gfElapsedTime * 10.5f;
		if (nResidual == 3) fShift = +gfElapsedTime * 10.5f;
		input.uv.x += fShift;
	}
#endif
	output.uv = input.uv;

	output.textureID = (int)input.billboardInfo.w - 1;

	return(output);
}

Texture2D<float4> gtxtBillboardTextures[7] : register(t10);

float4 PSBillboardInstancing(VS_BILLBOARD_INSTANCING_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtBillboardTextures[NonUniformResourceIndex(input.textureID)].Sample(gSamplerState, input.uv);

	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
