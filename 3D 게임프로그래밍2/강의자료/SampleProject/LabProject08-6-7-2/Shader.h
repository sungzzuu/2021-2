//-----------------------------------------------------------------------------
// File: Shader.h
//-----------------------------------------------------------------------------

#pragma once

#include "Object.h"
#include "Camera.h"

struct LIGHT;

class CShader
{
public:
	CShader();
	virtual ~CShader();

private:
	int								m_nReferences = 0;

public:
	void AddRef(){ m_nReferences++; }
	void Release(){ if (--m_nReferences <= 0) delete this; }

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob **ppd3dShaderBlob);

	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE d3dPrimitiveTopology, UINT nRenderTargets, DXGI_FORMAT *pdxgiRtvFormats, DXGI_FORMAT dxgiDsvFormat);

	void CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, int nConstantBufferViews, int nShaderResourceViews);
	void CreateConstantBufferViews(ID3D12Device *pd3dDevice, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	void CreateShaderResourceViews(ID3D12Device* pd3dDevice, CTexture* pTexture, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex);
	void CreateShaderResourceViews(ID3D12Device* pd3dDevice, int nResources, ID3D12Resource** ppd3dResources, DXGI_FORMAT *pdxgiSrvFormats);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext=NULL){ }
	virtual void AnimateObjects(float fTimeElapsed){ }
	virtual void ReleaseObjects(){ }

	virtual void ReleaseUploadBuffers();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void OnPostRender(ID3D12GraphicsCommandList *pd3dCommandList);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart(){ return(m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart(){ return(m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart()); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle(){ return(m_d3dCbvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle(){ return(m_d3dCbvGPUDescriptorStartHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle(){ return(m_d3dSrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle(){ return(m_d3dSrvGPUDescriptorStartHandle); }

public:
	ID3D12DescriptorHeap			*m_pd3dCbvSrvDescriptorHeap = NULL;

protected:
	ID3D12PipelineState				*m_pd3dPipelineState = NULL;

	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dCbvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dCbvGPUDescriptorStartHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dSrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dSrvGPUDescriptorStartHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dSrvCPUDescriptorNextHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dSrvGPUDescriptorNextHandle;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CPlayerShader : public CShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CIlluminatedShader : public CShader
{
public:
	CIlluminatedShader();
	virtual ~CIlluminatedShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
};

class CObjectsShader : public CIlluminatedShader
{
public:
	CObjectsShader();
	virtual ~CObjectsShader();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext=NULL);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void ReleaseObjects();

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void OnPostRender(ID3D12GraphicsCommandList *pd3dCommandList);

	BoundingBox CalculateBoundingBox();

public:
	CGameObject						**m_ppObjects = 0;
	int								m_nObjects = 0;
};

struct TOOBJECTSPACEINFO
{
	XMFLOAT4X4						m_xmf4x4ToTexture;
	XMFLOAT4						m_xmf4Position;
};

struct TOLIGHTSPACES
{
	TOOBJECTSPACEINFO				m_pToLightSpaces[MAX_LIGHTS];
};

class CDepthRenderShader : public CIlluminatedShader
{
public:
	CDepthRenderShader(CObjectsShader* pObjectsShader, LIGHT *pLights);
	virtual ~CDepthRenderShader();

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();

	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext=NULL);
	virtual void ReleaseObjects();

	void PrepareShadowMap(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

protected:
	CTexture						*m_pDepthTexture = NULL;

	CCamera*						m_ppDepthRenderCameras[MAX_DEPTH_TEXTURES];

	ID3D12DescriptorHeap			*m_pd3dRtvDescriptorHeap = NULL;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_pd3dRtvCPUDescriptorHandles[MAX_DEPTH_TEXTURES];

	ID3D12DescriptorHeap*			m_pd3dDsvDescriptorHeap = NULL;
	ID3D12Resource					*m_pd3dDepthBuffer = NULL;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dDsvDescriptorCPUHandle;

	XMMATRIX						m_xmProjectionToTexture;

public:
	CTexture* GetDepthTexture() { return(m_pDepthTexture); }
	ID3D12Resource* GetDepthTextureResource(UINT nIndex) { return(m_pDepthTexture->GetResource(nIndex)); }

public:
	CObjectsShader*					m_pObjectsShader = NULL;

protected:
	LIGHT							*m_pLights = NULL;

	TOLIGHTSPACES					*m_pToLightSpaces;

	ID3D12Resource					*m_pd3dcbToLightSpaces = NULL;
	TOLIGHTSPACES					*m_pcbMappedToLightSpaces = NULL;
};

class CShadowMapShader : public CIlluminatedShader
{
public:
	CShadowMapShader(CObjectsShader* pObjectsShader);
	virtual ~CShadowMapShader();

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext=NULL);
	virtual void AnimateObjects(float fTimeElapsed) { }
	virtual void ReleaseObjects();

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

public:
	CObjectsShader*					m_pObjectsShader = NULL;

	CTexture*						m_pDepthTexture = NULL;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTextureToViewportShader : public CShader
{
public:
	CTextureToViewportShader();
	virtual ~CTextureToViewportShader();

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext=NULL);
	virtual void ReleaseObjects();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

protected:
	CTexture*						m_pDepthTexture = NULL;
};

