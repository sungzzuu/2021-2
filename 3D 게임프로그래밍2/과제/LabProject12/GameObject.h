#pragma once
#include "Mesh.h"
class CShader;
class CCamera;
class CPlayer;
class CObjectsShader;

class CGameObject
{
public:
	CGameObject(int nMeshes=1);
	virtual ~CGameObject();
private:
	int							m_nReferences = 0;
public:
	void						AddRef() { m_nReferences++; }
	void						Release() { if (--m_nReferences <= 0) delete this; }
protected:
	XMFLOAT4X4					m_xmf4x4World;
	CShader*					m_pShader = NULL;
	//게임 객체는 여러 개의 메쉬를 포함하는 경우 게임 객체가 가지는 메쉬들에 대한 포인터와 그 개수이다.
	CMesh**						m_ppMeshes = NULL;
	int							m_nMeshes = 0;
	// 객체 스케일 지정
	XMFLOAT3					m_f3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT3					m_xmf3MoveDir = XMFLOAT3(0.0f, 0.0f, 1.0f);

	CObjectsShader*				m_pObjectsShader;

public:
	BoundingOrientedBox			m_xmOOBB;
	XMFLOAT4					m_xmf4HitColor;
	float						m_fHitTime = 0.f;
	bool						m_bHit = false;
	int							m_iHp;
public:
	void ReleaseUploadBuffers(); 
	virtual void SetMesh(int nIndex, CMesh* pMesh);
	virtual void SetShader(CShader* pShader);
	virtual void SetScale(XMFLOAT3 xmf3scale);
	virtual XMFLOAT3 GetScale() {return XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._22, m_xmf4x4World._33);}
	virtual bool Animate(float fTimeElapsed);
	virtual void OnPrepareRender();
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	void UpdateBoundingBox();
public:
	//상수 버퍼를 생성한다. 
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList 
	*pd3dCommandList);
	//상수 버퍼의 내용을 갱신한다. 
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	//게임 객체의 월드 변환 행렬에서 위치 벡터와 방향(x-축, y-축, z-축) 벡터를 반환한다. 
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	XMFLOAT3 GetDir();
	//게임 객체의 위치를 설정한다. 
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	//게임 객체를 로컬 x-축, y-축, z-축 방향으로 이동한다.
	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	// Dir 방향으로 게임 객체 이동
	void MoveByDir(float fDistance = 1.0f);
	//게임 객체를 회전(x-축, y-축, z-축)한다. 
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void SetDir(XMFLOAT3 vDir) { m_xmf3MoveDir = vDir; }
	void SetLook(XMFLOAT3 vLook);

	// 쉐이더 설정
	void SetObjectsShader(CObjectsShader* pShader) { m_pObjectsShader = pShader; }

	// 피격 처리
	void SetHit(bool bHit, XMFLOAT4 xmColor);
};

class CRotatingFlagObject : public CGameObject
{
public:
	enum STATE{NORMAL, DOWN, FINISH, END};
public:
	CRotatingFlagObject(int nMeshes = 1);
	virtual ~CRotatingFlagObject();
private:
	XMFLOAT3	m_xmf3RotationAxis;
	float		m_fRotationSpeed;
	STATE		m_eState;
	float		m_fOriginY;
	bool		m_bMyTeam;
public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) {m_xmf3RotationAxis = xmf3RotationAxis;}
	virtual bool Animate(float fTimeElapsed);
	STATE GetState() { return m_eState; }
	void SetState(STATE _eState);
	void SetMyTeam(bool bMyTeam) { m_bMyTeam = bMyTeam; }
	bool GetMyTeam() { return m_bMyTeam; }
};

// 날아오는 비행기
// 타겟 정해주기
// DIR 정해주기
class CEnemyFlyShip : public CGameObject
{
public:
	enum STATE
	{
		BEGIN, SPAWN, END
	};

public:
	CEnemyFlyShip(int nMeshes = 1);
	virtual ~CEnemyFlyShip();

private:
	STATE			m_eState;
	XMFLOAT3		m_pTargetPos;
	float			m_fSpeed;
	float			m_fSpawnTime = 0.f;
	bool			m_bSpawn = false;

public:
	STATE			GetState() { return m_eState; }
	void			SetState(STATE _eState) { m_eState = _eState; }
	void			SetTargetPos(XMFLOAT3 pos) { m_pTargetPos = pos; }
	void			SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	virtual bool	Animate(float fTimeElapsed);
	bool			SpawnCheck();

};



class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int
		nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4
		xmf4Color);
	virtual ~CHeightMapTerrain();
private:
	//지형의 높이 맵으로 사용할 이미지이다. 
	CHeightMapImage *m_pHeightMapImage;
	
	//높이 맵의 가로와 세로 크기이다. 
	int m_nWidth;
	int m_nLength;
	//지형을 실제로 몇 배 확대할 것인가를 나타내는 스케일 벡터이다. 
	XMFLOAT3 m_xmf3Scale;

public:
	//지형의 높이를 계산하는 함수이다(월드 좌표계). 높이 맵의 높이에 스케일의 y를 곱한 값이다. 
	float GetHeight(float x, float z) { return(m_pHeightMapImage->GetHeight(x / m_xmf3Scale.x,
		z / m_xmf3Scale.z)* m_xmf3Scale.y);}

	//지형의 법선 벡터를 계산하는 함수이다(월드 좌표계). 높이 맵의 법선 벡터를 사용한다. 
	XMFLOAT3 GetNormal(float x, float z) { 
		return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x),
		int(z / m_xmf3Scale.z))); }

	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	//지형의 크기(가로/세로)를 반환한다. 높이 맵의 크기에 스케일을 곱한 값이다. 
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }

};

class CTerrainObject : public CGameObject
{
public:
	CTerrainObject(void* pContext, float fMeshHeightHalf, int nMeshes = 1);
	virtual ~CTerrainObject();

	//플레이어의 위치가 바뀔 때마다 호출되는 함수와 그 함수에서 사용하는 정보를 설정하는 함수이다.
	virtual void OnObjectUpdateCallback(float fTimeElapsed);
	void SetTerrain(CHeightMapTerrain* pTerrain) { m_pTerrain = pTerrain; }
	void SetMeshHeightHalf(float fheight) { m_fMeshHeightHalf = fheight; }
	virtual bool Animate(float fTimeElapsed) {
		return OBJ_NONE;
	};

protected:
	CHeightMapTerrain*				m_pTerrain;
	float							m_fMeshHeightHalf;

};
// 적 오브젝트
class CEnemy : public CTerrainObject
{
public:
	enum STATE { BEGIN, NORMAL };
	CEnemy(void* pContext, float fMeshHeightHalf, int nMeshes = 1);
	virtual ~CEnemy();

public:
	virtual bool	Animate(float fTimeElapsed);
protected:
	STATE		m_eState;
	float		m_fRotationSpeed;
	float		m_fDownSpeed;
	float		m_fScale;
	float		m_fBulletCreateTime;
};

// 따라오는 적 오브젝트
class CFollowingEnemy : public CEnemy
{
public:
	CFollowingEnemy(void* pContext, float fMeshHeightHalf, CPlayer* pPlayer, int nMeshes = 1);
	virtual ~CFollowingEnemy();
public:
	virtual bool	Animate(float fTimeElapsed);
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
private:
	float				m_fSpeed;
	CPlayer*			m_pPlayer;

};


// 아군 오브젝트
class CMyTeamObject : public CTerrainObject
{
public:
	CMyTeamObject(void* pContext, void* pPlayer, XMFLOAT3 xmf3Offset, float fMeshHeightHalf, int nMeshes = 1);
	virtual ~CMyTeamObject();
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	virtual bool Animate(float fTimeElapsed);

protected:
	CPlayer*						m_pPlayer;
	XMFLOAT3						m_xmf3Offset;
	float							m_fBulletCreateTime;
};

// 아군 지상 공격 오브젝트 죽지 않음
// 타겟 으로 총알 발사
// 지상 공격만 함
class CMyTeamShip : public CMyTeamObject
{
public:
	CMyTeamShip(void* pContext, void* pPlayer, XMFLOAT3 xmf3Offset, float fMeshHeightHalf, int nMeshes = 1);
	virtual ~CMyTeamShip();
	virtual bool Animate(float fTimeElapsed);
	void SetTarget(CGameObject* pTarget) { m_pTarget = pTarget; }
private:
	CGameObject*					m_pTarget;
	float							m_fBulletCreateTime;
};


// Dir로 이동하는 총알
class CBullet : public CGameObject
{
public:
	CBullet(int nMeshes=1);
	virtual ~CBullet();
	virtual bool Animate(float fTimeElapsed);
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }

private:
	float			m_fSpeed;
	float			m_fCreateTime = 0.f;

};

// 공중 적 오브젝트
class CUFOEnemy : public CGameObject
{
public:
	CUFOEnemy(CPlayer* pPlayer, float fRadius, int nMeshes = 1);
	virtual ~CUFOEnemy();

public:
	virtual bool	Animate(float fTimeElapsed);
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetOriginPos(XMFLOAT3 xmf3pos) { m_xmf3OriginPos = xmf3pos; }
	void SetPlayerShip(CMyTeamShip* pPlayerShip) { m_pPlayerShip = pPlayerShip; }
private:
	CPlayer* m_pPlayer;
	float				m_fSpeed;
	float				m_fBulletCreateTime;
	float				m_fAngle;
	float				m_fRadius;
	XMFLOAT3			m_xmf3OriginPos;
	CMyTeamShip* m_pPlayerShip;
};