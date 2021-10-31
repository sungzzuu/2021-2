#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Camera.h"

//게임 객체의 정보를 셰이더에게 넘겨주기 위한 구조체(상수 버퍼)이다. 
struct CB_GAMEOBJECT_INFO
{
	XMFLOAT4X4 m_xmf4x4World;
};

class CShader
{
public:
	CShader();
	virtual ~CShader();
private:
	int m_nReferences = 0;
public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR* pszFileName, LPCSTR pszShaderName,
		LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
		* pd3dGraphicsRootSignature);
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList,
		XMFLOAT4X4* pxmf4x4World);


	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

protected:
	//파이프라인 상태 객체들의 리스트(배열)이다. 
	ID3D12PipelineState **m_ppd3dPipelineStates = NULL;
	int m_nPipelineStates = 0;
};

class CPlayerShader : public CShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
		* pd3dGraphicsRootSignature);
};

class CObjectsShader : public CShader
{
public:
	CObjectsShader();
	virtual ~CObjectsShader();
	virtual void InitBuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void* pContext);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void ReleaseObjects();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
		* pd3dGraphicsRootSignature);
	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

public:
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() { return m_pPlayer; }
	void SetTerrain(CHeightMapTerrain* pTerrain) { m_pTerrain = pTerrain; }
	// 아군 생성 함수
	void BuildMyTeam();
	// 적군 생성 함수
	void BuildEnemyFlyShip(XMFLOAT3 spotPos);
	void BuildEnemy(XMFLOAT3 spotPos);
	// 충돌체크 함수
	void Collision_Check();

	// 외부에서 객체 추가하는 함수
	void AddObject(OBJ::OBJID _eID, CGameObject* pObject);

	// 플레이어 총알 추가하는 함수
	void AddPlayerBullet();

	// 리스트에 오브젝트가 몇개인지 확인 
	int Get_listSize(OBJ::OBJID _eID) { return m_listObjects[_eID].size(); }

	void CheckEnding(float fTimeElapsed);
	void Cheat();
protected:
	list<CGameObject*>						m_listObjects[OBJ::END_OBJID];
	CPlayer*								m_pPlayer;
	CHeightMapTerrain						*m_pTerrain;
	int										m_iMyTeamBuildNum=0;
	XMFLOAT3								m_xmf3EnemyBasePos;
	XMFLOAT3								m_xmf3MyBasePos;
	int										n_iEnemyBuildNum = 0;
	CMyTeamShip*							m_myTeamShip[2];
protected:
	// 메쉬를 미리 만들어 놓는다.
	CCubeMeshDiffused*						m_pCubeMyTeamMesh;			// 아군 메쉬
	CCubeMeshDiffused*						m_pCubeMySpotMesh;			// 아군 지점 메쉬
	CCubeMeshDiffused*						m_pCubeEnemySpotMesh;		// 적군 지점 메쉬
	CModelMeshDiffused*						m_pEnemyFlyerShipMesh;		// 적군 비행기 메쉬
	CModelMeshDiffused*						m_pBaseMesh;				// 기지 메쉬
	CCubeMeshDiffused*						m_pCubeEnemyMesh;			// 적군 메쉬
	CCubeMeshDiffused*						m_pCubeEnemyBulletMesh;		// 적군 총알 메쉬
	CCubeMeshDiffused*						m_pCubePlayerBulletMesh;	// 플레이어 총알 메쉬
	CCubeMeshDiffused*						m_pCubeWaterMesh;			// 물 메쉬
	CModelMeshDiffused*						m_pMyTeamFlyerShipMesh;		// 플레이어 비행기 메쉬
	CModelMeshDiffused*						m_pEnemyUFOMesh;			// 적 UFO 메쉬

};

class CTerrainShader : public CShader
{
public:
	CTerrainShader();
	virtual ~CTerrainShader();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
		* pd3dGraphicsRootSignature);
};