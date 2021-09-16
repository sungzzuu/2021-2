#pragma once
#include "Timer.h"

// Direct3D 디바이스 생성관리, 객체 생성과 관리, 입력, 애니메이션
class CGameFramework
{
private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;

	int							m_nWndClientWidth;
	int							m_nWndClientHeight;

	// IDXGIFactory4: https://ssinyoung.tistory.com/33
	IDXGIFactory4*				m_pdxgiFactory;
	IDXGISwapChain3*			m_pdxgiSwapChain;			// 디스플레이 제어
	ID3D12Device*				m_pd3dDevice;				// 리소스 생성

	bool						m_bMsaa4xEnable = false;	// 4X MSAA* 품질 수준 지원 여부
	UINT						m_nMsaa4xQualityLevels = 0;	// 다중 샘플링 활성화 및 레벨 설정
	static const UINT			m_nSwapChainBuffers = 2;	// 후면 버퍼 개수 (static const: 선언 동시 값 설정)
	UINT						m_nSwapChainBufferIndex;	// 현재 후면 버퍼 인덱스

	// 렌더 타겟
	ID3D12Resource*				m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers]; // 버퍼
	ID3D12DescriptorHeap*		m_pd3dRtvDescriptorHeap;						// 포인터
	UINT						m_nRtvDescriptorIncrementSize;					// 크기

	// 깊이-스텐실
	ID3D12Resource*				m_pd3dDepthStencilBuffer;
	ID3D12DescriptorHeap*		m_pd3dDsvDescriptorHeap;
	UINT						m_nDsvDescriptorIncrementSize;

	// 명령
	ID3D12CommandQueue*			m_pd3dCommandQueue;
	ID3D12CommandAllocator*		m_pd3dCommandAllocator;
	ID3D12GraphicsCommandList*	m_pd3dCommandList;

	// 그래픽스 파이프라인 상태
	ID3D12PipelineState*		m_pd3dPipelineState;

	// 펜스
	ID3D12Fence*				m_pd3dFence;
	UINT64						m_nFenceValue;
	HANDLE						m_hFenceEvent;

	// 뷰포트, 시저 사각형
	D3D12_VIEWPORT				m_d3dViewport;
	D3D12_RECT					m_d3dScissorRect;

private:
	CGameTimer	m_GameTimer;
	_TCHAR		m_pszFrameRate[50];


public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd); // 프레임워크 초기화

	void OnDestroy();

	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRenderTargetViews();
	void CreateDepthStencilView();

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	void WaitForGpuComplete();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	// 전체화면 만드는 함수
	void ChangeSwapChainState();


};

