///-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"
#include "DDSTextureLoader12.h"
#include "Scene.h"

CShader::CShader()
{
	m_d3dSrvCPUDescriptorStartHandle.ptr = NULL;
	m_d3dSrvGPUDescriptorStartHandle.ptr = NULL;
}

CShader::~CShader()
{
	if (m_pd3dCbvSrvDescriptorHeap) m_pd3dCbvSrvDescriptorHeap->Release();
	if (m_pd3dPipelineState) m_pd3dPipelineState->Release();
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreateGeometryShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob)
{
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob *pd3dErrorBlob = NULL;
	::D3DCompileFromFile(pszFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, NULL);
	//	char *pErrorString = (char *) pd3dErrorBlob->GetBufferPointer();

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();

	return(d3dShaderByteCode);
}

D3D12_INPUT_LAYOUT_DESC CShader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = NULL;
	d3dInputLayoutDesc.NumElements = 0;

	return(d3dInputLayoutDesc);
}

D3D12_RASTERIZER_DESC CShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

D3D12_DEPTH_STENCIL_DESC CShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return(d3dDepthStencilDesc);
}

D3D12_BLEND_DESC CShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

void CShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, D3D12_PRIMITIVE_TOPOLOGY_TYPE d3dPrimitiveTopology, UINT nRenderTargets, DXGI_FORMAT* pdxgiRtvFormats, DXGI_FORMAT dxgiDsvFormat)
{	
	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dGeometryShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.GS = CreateGeometryShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = d3dPrimitiveTopology;
	d3dPipelineStateDesc.NumRenderTargets = nRenderTargets;
	for (UINT i = 0; i < nRenderTargets; i++) d3dPipelineStateDesc.RTVFormats[i] = (pdxgiRtvFormats) ? pdxgiRtvFormats[i] : DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = dxgiDsvFormat;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_pd3dPipelineState);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dGeometryShaderBlob) pd3dGeometryShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CShader::CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);

	m_d3dSrvCPUDescriptorNextHandle = m_d3dSrvCPUDescriptorStartHandle;
	m_d3dSrvGPUDescriptorNextHandle = m_d3dSrvGPUDescriptorStartHandle;
}

void CShader::CreateConstantBufferViews(ID3D12Device *pd3dDevice, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		D3D12_CPU_DESCRIPTOR_HANDLE d3dCbvCPUDescriptorHandle;
		d3dCbvCPUDescriptorHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * j);
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, d3dCbvCPUDescriptorHandle);
	}
}

void CShader::CreateShaderResourceViews(ID3D12Device* pd3dDevice, CTexture* pTexture, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex)
{
	m_d3dSrvCPUDescriptorNextHandle.ptr += (::gnCbvSrvDescriptorIncrementSize * nDescriptorHeapIndex);
	m_d3dSrvGPUDescriptorNextHandle.ptr += (::gnCbvSrvDescriptorIncrementSize * nDescriptorHeapIndex);

	int nTextures = pTexture->GetTextures();
	UINT nTextureType = pTexture->GetTextureType();
	for (int i = 0; i < nTextures; i++)
	{
		ID3D12Resource* pShaderResource = pTexture->GetResource(i);
		D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = pTexture->GetShaderResourceViewDesc(i);
		pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorNextHandle);
		m_d3dSrvCPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
		pTexture->SetSrvGpuDescriptorHandle(i, m_d3dSrvGPUDescriptorNextHandle);
		m_d3dSrvGPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
	}
	int nRootParameters = pTexture->GetRootParameters();
	for (int i = 0; i < nRootParameters; i++) pTexture->SetRootParameterIndex(i, nRootParameterStartIndex + i);
}

void CShader::CreateShaderResourceViews(ID3D12Device* pd3dDevice, int nResources, ID3D12Resource** ppd3dResources, DXGI_FORMAT *pdxgiSrvFormats)
{
	for (int i = 0; i < nResources; i++)
	{
		if (ppd3dResources[i])
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
			d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			d3dShaderResourceViewDesc.Format = pdxgiSrvFormats[i];
			d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			d3dShaderResourceViewDesc.Texture2D.MipLevels = 1;
			d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
			d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			pd3dDevice->CreateShaderResourceView(ppd3dResources[i], &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorNextHandle);

			m_d3dSrvCPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
			m_d3dSrvGPUDescriptorNextHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
		}
	}
}

void CShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CShader::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
{
}

void CShader::ReleaseShaderVariables()
{
}

void CShader::ReleaseUploadBuffers()
{
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_pd3dPipelineState) pd3dCommandList->SetPipelineState(m_pd3dPipelineState);
	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	UpdateShaderVariables(pd3dCommandList);
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList);
}

void CShader::OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] ={ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSPlayer", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSPlayer", "ps_5_1", ppd3dShaderBlob));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIlluminatedShader::CIlluminatedShader()
{
}

CIlluminatedShader::~CIlluminatedShader()
{
}

D3D12_INPUT_LAYOUT_DESC CIlluminatedShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] ={ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CIlluminatedShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSLighting", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CIlluminatedShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSLighting", "ps_5_1", ppd3dShaderBlob));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CObjectsShader::CObjectsShader()
{
}

CObjectsShader::~CObjectsShader()
{
}

BoundingBox CObjectsShader::CalculateBoundingBox()
{
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->CalculateBoundingBox();

	BoundingBox xmBoundingBox = m_ppObjects[0]->m_xmBoundingBox;
	for (int i = 1; i < m_nObjects; i++)BoundingBox::CreateMerged(xmBoundingBox, xmBoundingBox, m_ppObjects[i]->m_xmBoundingBox);

	return(xmBoundingBox);
}

void CObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	m_nObjects = 11;
	m_ppObjects = new CGameObject*[m_nObjects];

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CAirPlaneMeshIlluminated *pAirPlaneMesh = new CAirPlaneMeshIlluminated(pd3dDevice, pd3dCommandList, 40.0f, 40.0f, 4.0f);

	CMaterial *pMaterial = new CMaterial();
	pMaterial->SetReflection(2);

	CRotatingObject *pRoatingAirPlane = new CRotatingObject(1);
	pRoatingAirPlane->SetMesh(0, pAirPlaneMesh);
	pRoatingAirPlane->SetMaterial(pMaterial);
	pRoatingAirPlane->SetPosition(100.0f, 50.0f, 120.0f);
	pRoatingAirPlane->Rotate(90.0f, 0.0f, 0.0f);
	pRoatingAirPlane->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
	pRoatingAirPlane->SetRotationSpeed(0.0f);
	m_ppObjects[0] = pRoatingAirPlane;

	CCubeMeshIlluminated *pCubeMesh = new CCubeMeshIlluminated(pd3dDevice, pd3dCommandList, 30.0f, 30.0f, 30.0f);

	pMaterial = new CMaterial();
	pMaterial->SetReflection(3);

	CRotatingObject *pRotatingCube;

	pRotatingCube = new CRotatingObject(1);
	pRotatingCube->SetMesh(0, pCubeMesh);

	pRotatingCube->SetMaterial(pMaterial);
	pRotatingCube->SetPosition(-100.0f, 145.0f, -120.0f);
	pRotatingCube->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pRotatingCube->SetRotationSpeed(0.0f);
	m_ppObjects[1] = pRotatingCube;

	pRotatingCube = new CRotatingObject(1);
	pRotatingCube->SetMesh(0, pCubeMesh);

	pRotatingCube->SetMaterial(pMaterial);
	pRotatingCube->SetPosition(+150.0f, 40.0f, -180.0f);
	pRotatingCube->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pRotatingCube->SetRotationSpeed(0.0f);
	m_ppObjects[2] = pRotatingCube;

	pRotatingCube = new CRotatingObject(1);
	pRotatingCube->SetMesh(0, pCubeMesh);

	pRotatingCube->SetMaterial(pMaterial);
	pRotatingCube->SetPosition(+100.0f, 80.0f, -160.0f);
	pRotatingCube->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pRotatingCube->SetRotationSpeed(0.0f);
	m_ppObjects[3] = pRotatingCube;

	pRotatingCube = new CRotatingObject(1);
	pRotatingCube->SetMesh(0, pCubeMesh);

	pRotatingCube->SetMaterial(pMaterial);
	pRotatingCube->SetPosition(+130.0f, 15.0f, +170.0f);
	pRotatingCube->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pRotatingCube->SetRotationSpeed(0.0f);
	m_ppObjects[4] = pRotatingCube;

	pMaterial = new CMaterial();
	pMaterial->SetReflection(4);

	m_ppObjects[5] = new CGameObject(1);
	m_ppObjects[5]->SetMesh(0, pCubeMesh);
	m_ppObjects[5]->SetMaterial(pMaterial);
	m_ppObjects[5]->SetPosition(30.0f, 30.0f, -135.0f);

	pMaterial = new CMaterial();
	pMaterial->SetReflection(5);

	m_ppObjects[6] = new CGameObject(1);
	m_ppObjects[6]->SetMesh(0, pAirPlaneMesh);
	m_ppObjects[6]->SetMaterial(pMaterial);
	m_ppObjects[6]->Rotate(90.0f, 0.0f, 0.0f);
	m_ppObjects[6]->SetPosition(XMFLOAT3(-(_PLANE_WIDTH * 0.5f), 160.0f, 0.0f));

	pRotatingCube = new CRotatingObject(1);
	pRotatingCube->SetMesh(0, pCubeMesh);

	pRotatingCube->SetMaterial(pMaterial);
	pRotatingCube->SetPosition(+230.0f, 15.0f, +290.0f);
	pRotatingCube->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pRotatingCube->SetRotationSpeed(0.0f);
	m_ppObjects[7] = pRotatingCube;

	pRotatingCube = new CRotatingObject(1);
	pRotatingCube->SetMesh(0, pCubeMesh);

	pRotatingCube->SetMaterial(pMaterial);
	pRotatingCube->SetPosition(-230.0f, 155.0f, +290.0f);
	pRotatingCube->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pRotatingCube->SetRotationSpeed(0.0f);
	m_ppObjects[8] = pRotatingCube;

	pRotatingCube = new CRotatingObject(1);
	pRotatingCube->SetMesh(0, pCubeMesh);

	pRotatingCube->SetMaterial(pMaterial);
	pRotatingCube->SetPosition(-130.0f, 55.0f, +200.0f);
	pRotatingCube->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pRotatingCube->SetRotationSpeed(0.0f);
	m_ppObjects[9] = pRotatingCube;

	m_ppObjects[10] = new CGameObject(1);
	m_ppObjects[10]->SetMesh(0, pCubeMesh);
	m_ppObjects[10]->SetMaterial(pMaterial);
	m_ppObjects[10]->SetPosition(XMFLOAT3(-30.0f, 15.0f, 0.0f));
}

void CObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}
}

void CObjectsShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}

void CObjectsShader::ReleaseUploadBuffers()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
	}
}

void CObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CIlluminatedShader::Render(pd3dCommandList, pCamera);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->UpdateShaderVariables(pd3dCommandList);
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}
}

void CObjectsShader::OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlanarShadowShader::CPlanarShadowShader(CObjectsShader* pObjectsShader, LIGHT *pLights)
{
	m_pObjectsShader = pObjectsShader;
	m_pLights = pLights;
}

CPlanarShadowShader::~CPlanarShadowShader()
{
}

D3D12_INPUT_LAYOUT_DESC CPlanarShadowShader::CreateInputLayout()
{
	return(m_pObjectsShader->CreateInputLayout());
}

D3D12_DEPTH_STENCIL_DESC CPlanarShadowShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return(d3dDepthStencilDesc);
}

D3D12_BLEND_DESC CPlanarShadowShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_SHADER_BYTECODE CPlanarShadowShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSPlanarShadow", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPlanarShadowShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSPlanarShadow", "ps_5_1", ppd3dShaderBlob));
}

void CPlanarShadowShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	CShader::Render(pd3dCommandList, pCamera);

	XMFLOAT4 xmf4Plane(0.0f, 1.0f, 0.0f, -0.1f);

	for (int j = 0; j < MAX_LIGHTS; j++)
	{
		if (m_pLights[j].m_bEnable)
		{
			XMFLOAT4 xmf4Light = (m_pLights[j].m_nType == DIRECTIONAL_LIGHT) ? XMFLOAT4(-m_pLights[j].m_xmf3Direction.x, -m_pLights[j].m_xmf3Direction.y, -m_pLights[j].m_xmf3Direction.z, 0.0f) : XMFLOAT4(m_pLights[j].m_xmf3Position.x, m_pLights[j].m_xmf3Position.y, m_pLights[j].m_xmf3Position.z, 1.0f);
			XMMATRIX xmmtxShadow = XMMatrixShadow(XMLoadFloat4(&xmf4Plane), XMLoadFloat4(&xmf4Light));

			for (int i = 0; i < m_pObjectsShader->m_nObjects; i++)
			{
				if (m_pObjectsShader->m_ppObjects[i])
				{
					m_pObjectsShader->m_ppObjects[i]->UpdateShaderVariable(pd3dCommandList, &xmmtxShadow);
					m_pObjectsShader->m_ppObjects[i]->Render(pd3dCommandList, pCamera);
				}
			}
		}
	}
}

