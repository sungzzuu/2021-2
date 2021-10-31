#include "stdafx.h"
#include "Shader.h"

CShader::CShader()
{
	if (m_ppd3dPipelineStates)
	{
		for (int i = 0; i < m_nPipelineStates; i++) if (m_ppd3dPipelineStates[i])
			m_ppd3dPipelineStates[i]->Release();
		delete[] m_ppd3dPipelineStates;
	}
}

CShader::~CShader()
{
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
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID; // D3D12_FILL_MODE_WIREFRAME; 
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
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP; d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	return(d3dDepthStencilDesc);
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;
	return(d3dShaderByteCode);
}
D3D12_SHADER_BYTECODE CShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;
	return(d3dShaderByteCode);
}

//셰이더 소스 코드를 컴파일하여 바이트 코드 구조체를 반환한다.
D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR *pszFileName, LPCSTR 
pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob)
{
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#endif
	::D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderProfile,
		nCompileFlags, 0, ppd3dShaderBlob, NULL);
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();
	return(d3dShaderByteCode);
}

void CShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		__uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[0]);
	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();
	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[]
		d3dPipelineStateDesc.InputLayout.pInputElementDescs;

}

void CShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CShader::ReleaseShaderVariables()
{
}

void CShader::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	//파이프라인에 그래픽스 상태 객체를 설정한다.
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);
}
void CShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	OnPrepareRender(pd3dCommandList);
}

CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);

}

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",
		ppd3dShaderBlob));

}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1",
		ppd3dShaderBlob));

}

void CPlayerShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

}

CObjectsShader::CObjectsShader()
{
}

CObjectsShader::~CObjectsShader()
{
	ReleaseObjects();
}

void CObjectsShader::InitBuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
	ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{
	SetTerrain((CHeightMapTerrain*)pContext);
	/* 사용할 메쉬 미리 만들어 놓는 부분*/
	m_pCubeMyTeamMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 6.f, 6.f, 6.f, OBJ::BLUE); // 아군 메쉬
	m_pCubeMySpotMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 30.f, 50.f, 30.f, OBJ::BLUE); // 아군 스팟 메쉬
	m_pCubeEnemySpotMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 30.f, 50.f, 30.f, OBJ::RED); // 적군 스팟 메쉬

	m_pEnemyFlyerShipMesh = new CModelMeshDiffused(pd3dDevice, pd3dCommandList, "Models/FlyerPlayership.txt", true); // 적군 우주선 메쉬
	m_pBaseMesh = new CModelMeshDiffused(pd3dDevice, pd3dCommandList, "Models/ams_house3.bin", false); // 기지 메쉬
	m_pCubeEnemyMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 8.f, 8.f, 8.f, OBJ::RED);
	m_pCubeEnemyBulletMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 4.f, 4.f, 4.f, OBJ::YELLOW);
	m_pCubePlayerBulletMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 4.f, 4.f, 4.f, OBJ::GREEN);
	m_pCubeWaterMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList,1500.f, 50.f, 1500.f, OBJ::BLUE);
	m_pMyTeamFlyerShipMesh = new CModelMeshDiffused(pd3dDevice, pd3dCommandList, "Models/FlyerPlayership.txt", true);
	m_pEnemyUFOMesh = new CModelMeshDiffused(pd3dDevice, pd3dCommandList, "Models/UFO.bin", false);
	/* 아군 생성 지점 만드는 부분 */
	// 아군 지점 20,fheight, 100에 설치
	XMFLOAT3 pos[5];
	pos[0] = { 50, 0, 300 };
	pos[1] = { 500, 0, 100 };
	pos[2] = { 1000, 0, 500 };
	pos[3] = { 1500, 0, 600 };
	pos[4] = { 1800, 0, 200 };

	for(int i = 0; i < 5; ++i)
	{
		XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
		CRotatingFlagObject* pFlagObject = NULL;
		pFlagObject = new CRotatingFlagObject(1);
		pFlagObject->SetMesh(0, m_pCubeMySpotMesh);
		pFlagObject->SetPosition(pos[i].x, m_pTerrain->GetHeight(pos[i].x, pos[i].z) + 25.f, pos[i].z);
		pFlagObject->SetState(CRotatingFlagObject::NORMAL);
		pFlagObject->SetMyTeam(true);

		xmf3SurfaceNormal = m_pTerrain->GetNormal(pos[i].x, pos[i].z);
		xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
			xmf3SurfaceNormal);
		if (Vector3::IsZero(xmf3RotateAxis)) xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f,
			0.0f);
		float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
			xmf3SurfaceNormal));
		pFlagObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
		pFlagObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
		pFlagObject->SetRotationSpeed(36.0f);

		m_listObjects[OBJ::FLAG].push_back(pFlagObject);
	}
	//=========================================================================================================

	/* 적군 지점 생성*/
	pos[0] = { 100, 0, 700 };
	pos[1] = { 1100, 0, 1300 };
	pos[2] = { 1000, 0, 1700 };
	pos[3] = { 1600, 0, 1600 };

	for(int i = 0; i < 4; ++i)
	{
		XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
		CRotatingFlagObject* pFlagObject = NULL;
		pFlagObject = new CRotatingFlagObject(1);
		pFlagObject->SetMesh(0, m_pCubeEnemySpotMesh);
		pFlagObject->SetPosition(pos[i].x, m_pTerrain->GetHeight(pos[i].x, pos[i].z) +25.f, pos[i].z);
		pFlagObject->SetState(CRotatingFlagObject::NORMAL);
		pFlagObject->SetMyTeam(false);
		xmf3SurfaceNormal = m_pTerrain->GetNormal(pos[i].x, pos[i].z);
		xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
			xmf3SurfaceNormal);
		if (Vector3::IsZero(xmf3RotateAxis)) xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f,
			0.0f);
		float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
			xmf3SurfaceNormal));
		pFlagObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
		pFlagObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
		pFlagObject->SetRotationSpeed(36.0f);

		m_listObjects[OBJ::FLAG].push_back(pFlagObject);
	}
	//=========================================================================================================
	/* 아군, 적군 기지 생성 */
	CGameObject* pMyBase = new CGameObject(1);
	pMyBase->SetMesh(0, m_pBaseMesh);
	pMyBase->SetPosition(70, m_pTerrain->GetHeight(70, 70)-20.f, 70);
	pMyBase->SetScale(XMFLOAT3(30.f, 10.f, 30.f));
	m_xmf3MyBasePos = pMyBase->GetPosition();
	m_listObjects[OBJ::BASE].push_back(pMyBase);

	CGameObject* pEnemyBase = new CGameObject(1);
	pEnemyBase->SetMesh(0, m_pBaseMesh);
	pEnemyBase->Rotate(0.f, 180.f, 0.f);
	pEnemyBase->SetScale(XMFLOAT3(30.f, 10.f, 30.f));
	pEnemyBase->SetPosition(1900, m_pTerrain->GetHeight(1900, 1700) - 20.f, 1700);
	m_xmf3EnemyBasePos = pEnemyBase->GetPosition();

	m_listObjects[OBJ::BASE].push_back(pEnemyBase);
	//=========================================================================================================

	/* 물 생성 */
	CGameObject* pWater = new CGameObject(1);
	pWater->SetMesh(0, m_pCubeWaterMesh);
	pWater->SetPosition(1200.f,100.f, 1200.f);
	m_listObjects[OBJ::NONE].push_back(pWater);

	//pEnemyFlyShip->SetPosition();
	// 비행기 테스트 출력
	//XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
	//CRotatingFlagObject* pEnemyFlyerShip = NULL;
	//pEnemyFlyerShip = new CRotatingFlagObject(1);
	//pEnemyFlyerShip->SetMesh(0, m_pEnemyFlyerShipMesh);
	//pEnemyFlyerShip->SetPosition(0, m_pTerrain->GetHeight(0, 100) + 20.f, 100);
	//pEnemyFlyerShip->SetState(CRotatingFlagObject::NORMAL);
	//pEnemyFlyerShip->SetScale(XMFLOAT3(10.f, 10.f, 10.f));
	//xmf3SurfaceNormal = m_pTerrain->GetNormal(0, 100);
	//xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
	//	xmf3SurfaceNormal);
	//if (Vector3::IsZero(xmf3RotateAxis)) xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f,
	//	0.0f);
	//float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
	//	xmf3SurfaceNormal));
	//pEnemyFlyerShip->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
	//pEnemyFlyerShip->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pEnemyFlyerShip->SetRotationSpeed(36.0f);

	//m_listObjects[OBJ::FLAG].push_back(pEnemyFlyerShip);



	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CObjectsShader::AnimateObjects(float fTimeElapsed)
{
	for (int i = 0; i < OBJ::END_OBJID; ++i)
	{
		for (auto iter = m_listObjects[i].begin(); iter != m_listObjects[i].end();)
		{
			if (i == OBJ::FLYSHIP)
				if (dynamic_cast<CEnemyFlyShip*>(*iter)->SpawnCheck())
					BuildEnemy((*iter)->GetPosition());

			if ((*iter)->Animate(fTimeElapsed) == OBJ_DEAD)
			{
				SAFE_DELETE(*iter);
				iter = m_listObjects[i].erase(iter);
				cout << "객체 삭제" << endl;
			}
			else
				++iter;
		}
	}
	Collision_Check();
	CheckEnding(fTimeElapsed);
}

void CObjectsShader::ReleaseObjects()
{
	for (int i = 0; i < OBJ::END_OBJID; ++i)
	{
		for (auto& object : m_listObjects[i])
		{
			SAFE_DELETE(object);

		}
		m_listObjects[i].clear();

	}

	// 메쉬 삭제
	delete m_pCubeMyTeamMesh;		// 아군 메쉬
	delete m_pCubeMySpotMesh;		// 아군 지점 메쉬
	delete m_pCubeEnemySpotMesh;	// 적군 지점 메쉬
	delete m_pEnemyFlyerShipMesh;	// 적군 비행기 메쉬
	delete m_pBaseMesh;				// 기지 메쉬
	delete m_pCubeEnemyMesh;		// 적군 메쉬
	delete m_pCubeEnemyBulletMesh;	// 적군 총알 메쉬
}

D3D12_INPUT_LAYOUT_DESC CObjectsShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs; return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CObjectsShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",
		ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CObjectsShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1",
		ppd3dShaderBlob));
}

void CObjectsShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

}

void CObjectsShader::ReleaseUploadBuffers()
{
	for (int i = 0; i < OBJ::END_OBJID; ++i)
	{
		for (auto& object : m_listObjects[i])
		{
			object->ReleaseUploadBuffers();
		}
	}
}

void CObjectsShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	CShader::Render(pd3dCommandList, pCamera); 
	for (int i = 0; i < OBJ::END_OBJID; ++i)
	{
		for (auto& object : m_listObjects[i])
		{
			object->Render(pd3dCommandList, pCamera);
		}
	}
}

void CObjectsShader::BuildMyTeam()
{
	// 순서별로 생성 오프셋 다르게 해야함....
	XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
	CMyTeamObject* pTerrainObject = NULL;
	if (m_iMyTeamBuildNum == 0)
	{
		// 비행기 생성
		XMFLOAT3 offset = { -50.f, 0.f, -10.f };
		CMyTeamShip* pMyTeamShip = new CMyTeamShip(m_pTerrain, m_pPlayer, offset, 10.f);
		pMyTeamShip->SetMesh(0, m_pMyTeamFlyerShipMesh);
		pMyTeamShip->SetScale(XMFLOAT3(4.f, 4.f, 4.f));
		pMyTeamShip->Rotate(-20.f, 0.f, 0.f);
		XMFLOAT3 pos = {
			m_pPlayer->GetPosition().x + offset.x,
			m_pPlayer->GetPosition().y + offset.y,
			m_pPlayer->GetPosition().z + offset.z };

		pMyTeamShip->SetPosition(XMFLOAT3(pos.x, m_pTerrain->GetHeight(pos.x, pos.z) + 10.f, pos.z));
		pMyTeamShip->SetObjectsShader(this);
		m_listObjects[OBJ::MYTEAM].push_back(pMyTeamShip);
		m_myTeamShip[0] = pMyTeamShip;

		offset = { 50.f, 0.f, -10.f };
		pMyTeamShip = new CMyTeamShip(m_pTerrain, m_pPlayer, offset, 10.f);
		pMyTeamShip->SetMesh(0, m_pMyTeamFlyerShipMesh);
		pMyTeamShip->SetScale(XMFLOAT3(4.f, 4.f, 4.f));
		pMyTeamShip->Rotate(-20.f,0.f, 0.f);


		pos = {
			m_pPlayer->GetPosition().x + offset.x,
			m_pPlayer->GetPosition().y + offset.y,
			m_pPlayer->GetPosition().z + offset.z };

		pMyTeamShip->SetPosition(XMFLOAT3(pos.x, m_pTerrain->GetHeight(pos.x, pos.z) + 10.f, pos.z));
		pMyTeamShip->SetObjectsShader(this);
		m_listObjects[OBJ::MYTEAM].push_back(pMyTeamShip);
		m_myTeamShip[1] = pMyTeamShip;

	}
	for (int i = 0; i < 5; ++i)
	{
		XMFLOAT3 offset = { -20.f + i * 10.f,0.f, -10.f + m_iMyTeamBuildNum*(-10.f) };

		pTerrainObject = new CMyTeamObject(m_pTerrain, m_pPlayer, offset, 3.f);
		pTerrainObject->SetMesh(0, m_pCubeMyTeamMesh);

		XMFLOAT3 pos = {
			m_pPlayer->GetPosition().x + offset.x,
			m_pPlayer->GetPosition().y + offset.y,
			m_pPlayer->GetPosition().z + offset.z };

		pTerrainObject->SetPosition(XMFLOAT3(pos.x, m_pTerrain->GetHeight(pos.x, pos.z) + 3.f, pos.z));
		pTerrainObject->SetObjectsShader(this);

		m_listObjects[OBJ::MYTEAM].push_back(pTerrainObject);
	}
	m_iMyTeamBuildNum++;
	
}

void CObjectsShader::BuildEnemyFlyShip(XMFLOAT3 spotPos)
{
	// 비행기 먼저 만들고 -> 비행기 x,z 로부터 z 플러스 방향에서 100 떨어진곳에 생성
		/* 적군 우주선 생성*/
	CEnemyFlyShip* pEnemyFlyShip = new CEnemyFlyShip(1);
	pEnemyFlyShip->SetMesh(0, m_pEnemyFlyerShipMesh);
	pEnemyFlyShip->SetScale(XMFLOAT3(10.f, 10.f, 10.f));

	pEnemyFlyShip->SetSpeed(100.f);
	pEnemyFlyShip->SetState(CEnemyFlyShip::BEGIN);
	// 플레이어 바라보도록 Look 설정
	//XMFLOAT3 vLook = Vector3::Subtract(spotPos, XMFLOAT3(spotPos.x , spotPos.y + 60.f, spotPos.z));
	//vLook.y = 0.f;
	//vLook = Vector3::Normalize(vLook);

	//pEnemyFlyShip->SetLook(XMFLOAT3(0.f,0.f,-1.f));
	
	XMFLOAT3 vCreatePos = { spotPos.x + 200.f, spotPos.y + 60.f, spotPos.z + 200.f };


	XMFLOAT3 vLook = Vector3::Subtract(spotPos, vCreatePos);
	vLook.y = 0.f;
	vLook = Vector3::Normalize(vLook);
	pEnemyFlyShip->SetDir(vLook);

	// 플레이어 방향으로 회전
	float fAngle = Vector3::Angle(pEnemyFlyShip->GetLook(), vLook);
	pEnemyFlyShip->Rotate(0.f, 225.f,0.f);
	pEnemyFlyShip->SetPosition(vCreatePos);
	pEnemyFlyShip->SetTargetPos(spotPos);
	m_listObjects[OBJ::FLYSHIP].push_back(pEnemyFlyShip);

}

void CObjectsShader::BuildEnemy(XMFLOAT3 spotPos)
{
	switch (n_iEnemyBuildNum)
	{
	case 0:
		// 5마리 생성
		for (int i = 0; i < 5; ++i)
		{
			CEnemy* pEnemy = new CEnemy(m_pTerrain, 4.f, 1);
			pEnemy->SetMesh(0, m_pCubeEnemyMesh);
			pEnemy->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
			pEnemy->SetPosition(spotPos.x - 10.f + i * 20.f, spotPos.y, spotPos.z);
			pEnemy->SetObjectsShader(this);
			m_listObjects[OBJ::ENEMY].push_back(pEnemy);
		}
		break;
	case 1:
		for (int i = 0; i < 5; ++i)
		{
			// 따라오는 몬스터 
			CFollowingEnemy* pEnemy = new CFollowingEnemy(m_pTerrain, 4.f, m_pPlayer, 1);
			pEnemy->SetMesh(0, m_pCubeEnemyMesh);
			pEnemy->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
			pEnemy->SetPosition(spotPos.x - 60.f + i*30.f, spotPos.y, spotPos.z);
			pEnemy->SetObjectsShader(this);
			pEnemy->SetSpeed(20.f + 10.f * i);
			m_listObjects[OBJ::ENEMY].push_back(pEnemy);
		}
		break;
	case 2:
		// 15마리 생성
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				CEnemy* pEnemy = new CEnemy(m_pTerrain, 4.f, 1);
				pEnemy->SetMesh(0, m_pCubeEnemyMesh);
				pEnemy->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
				pEnemy->SetPosition(spotPos.x - 10.f + i * 20.f, spotPos.y, spotPos.z + j * 20.f);
				pEnemy->SetObjectsShader(this);
				m_listObjects[OBJ::ENEMY].push_back(pEnemy);
			}

		}
		break;
	case 3:
		for (int i = 0; i < 2; ++i)
		{
			CUFOEnemy* pEnemy = new CUFOEnemy(m_pPlayer,50.f + 20.f * i);
			pEnemy->SetMesh(0, m_pEnemyUFOMesh);
			pEnemy->SetScale(XMFLOAT3(5.f, 5.f, 5.f));
			pEnemy->SetPosition(spotPos.x , spotPos.y + 10.f*i, spotPos.z);
			pEnemy->SetObjectsShader(this);
			pEnemy->SetOriginPos(pEnemy->GetPosition());
			pEnemy->SetSpeed(1.f + 0.2f*i);
			pEnemy->SetPlayerShip(m_myTeamShip[i]);

			m_listObjects[OBJ::ENEMY].push_back(pEnemy);
			m_myTeamShip[i]->SetTarget(pEnemy);
		}
		break;
	default:
		break;
	}
	
	n_iEnemyBuildNum++;

}

void CObjectsShader::Collision_Check()
{
	// 플레이어와 플래그
	for (auto& flag : m_listObjects[OBJ::FLAG])
	{
		CRotatingFlagObject::STATE eState = dynamic_cast<CRotatingFlagObject*>(flag)->GetState();

		if (eState == CRotatingFlagObject::FINISH)
		{
			dynamic_cast<CRotatingFlagObject*>(flag)->SetState(CRotatingFlagObject::END);

			// 적팀인지 아닌지 확인
			if (dynamic_cast<CRotatingFlagObject*>(flag)->GetMyTeam())
				BuildMyTeam();
			else
				BuildEnemyFlyShip(flag->GetPosition());
		}

		if (flag->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB))
		{
			// 아군 플레이어 생성 및 플래그 내리기

			if ( eState == CRotatingFlagObject::NORMAL)
			{
				dynamic_cast<CRotatingFlagObject*>(flag)->SetState(CRotatingFlagObject::DOWN);
			}
			break;
		}
	}

	// 플레이어와 몬스터 총알
	for (auto iter = m_listObjects[OBJ::MONSTER_BULLET].begin(); iter != m_listObjects[OBJ::MONSTER_BULLET].end();)
	{
		if ((*iter)->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB))
		{
			m_pPlayer->SetHit(true, XMFLOAT4(1.f, 0.f, 0.f, 0.f));
			iter = m_listObjects[OBJ::MONSTER_BULLET].erase(iter);
			cout << "총알 충돌" << endl;
		}
		else
			++iter;
	}
	// 마이팀과 몬스터 총알
	for (auto& myTeam : m_listObjects[OBJ::MYTEAM])
	{
		for (auto iterBullet = m_listObjects[OBJ::MONSTER_BULLET].begin(); iterBullet != m_listObjects[OBJ::MONSTER_BULLET].end();)
		{
			if ((*iterBullet)->m_xmOOBB.Intersects(myTeam->m_xmOOBB))
			{
				cout << "마이팀 충돌" << endl;
				myTeam->SetHit(true, XMFLOAT4(1.f, 0.f, 0.f, 0.f));
				iterBullet = m_listObjects[OBJ::MONSTER_BULLET].erase(iterBullet);
			}
			else
				++iterBullet;
		}
	}
	// 몬스터와 플레이어 총알
	for (auto& enemy : m_listObjects[OBJ::ENEMY])
	{
		for (auto iterBullet = m_listObjects[OBJ::MY_BULLET].begin(); iterBullet != m_listObjects[OBJ::MY_BULLET].end();)
		{
			if ((*iterBullet)->m_xmOOBB.Intersects(enemy->m_xmOOBB))
			{
				cout << "몬스터 충돌" << endl;
				enemy->SetHit(true, XMFLOAT4(0.f,1.f,0.f,0.f));
				iterBullet = m_listObjects[OBJ::MY_BULLET].erase(iterBullet);
			}
			else
				++iterBullet;
		}
	}

}

void CObjectsShader::AddObject(OBJ::OBJID _eID, CGameObject* pObject)
{
	if (_eID == OBJ::MONSTER_BULLET)
	{
		pObject->SetMesh(0, m_pCubeEnemyBulletMesh);
	}
	else if (_eID == OBJ::MY_BULLET)
	{
		pObject->SetMesh(0, m_pCubePlayerBulletMesh);

	}
	m_listObjects[_eID].push_back(pObject);
}

void CObjectsShader::AddPlayerBullet()
{
	// 플레이어의 Look 방향으로
	XMFLOAT3 playerLook = m_pPlayer->GetLook();
	XMFLOAT3 playerPos = m_pPlayer->GetPosition();

	CBullet* pBullet = new CBullet(1);
	pBullet->SetMesh(0, m_pCubePlayerBulletMesh);
	pBullet->SetPosition(playerPos);
	pBullet->SetDir(playerLook);
	m_listObjects[OBJ::MY_BULLET].push_back(pBullet);
	//cout << playerLook.x <<" "<< playerLook.y << " " <<playerLook.z << endl;
}

void CObjectsShader::CheckEnding(float fTimeElapsed)
{
	// 몬스터가 없고 기지 다 내려가 있으면
	if (n_iEnemyBuildNum == 4 && 
		m_listObjects[OBJ::ENEMY].size() == 0 && 
		m_listObjects[OBJ::FLYSHIP].size() == 0 &&
		m_listObjects[OBJ::BASE].back()->GetPosition().y + 120.f > m_xmf3EnemyBasePos.y)
	{

		// 몬스터 기지 가라앉음
		m_listObjects[OBJ::BASE].back()->MoveUp(-10.f * fTimeElapsed);
	}
}

void CObjectsShader::Cheat()
{
	m_pPlayer->SetPosition(XMFLOAT3(1900.f, 100.f, 1600.f));
	n_iEnemyBuildNum = 4;

}

CTerrainShader::CTerrainShader()
{
}

CTerrainShader::~CTerrainShader()
{
}

D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs]; 
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",
		ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1",
		ppd3dShaderBlob));
}
void CTerrainShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
	* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}
