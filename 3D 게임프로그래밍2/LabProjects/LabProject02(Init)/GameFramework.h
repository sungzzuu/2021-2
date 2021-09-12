#pragma once

// Direct3D ����̽� ��������, ��ü ������ ����, �Է�, �ִϸ��̼�
class CGameFramework
{
private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;

	int							m_nWndClientWidth;
	int							m_nWndClientHeight;

	// IDXGIFactory4: https://ssinyoung.tistory.com/33
	IDXGIFactory4*				m_pdxgiFactory;
	IDXGISwapChain3*			m_pdxgiSwapChain;			// ���÷��� ����
	ID3D12Device*				m_pd3dDevice;				// ���ҽ� ����

	bool						m_bMsaa4xEnable = false;	// 4X MSAA* ǰ�� ���� ���� ����
	UINT						m_nMsaa4xQualityLevels = 0;	// ���� ���ø� Ȱ��ȭ �� ���� ����
	static const UINT			m_nSwapChainBuffers = 2;	// �ĸ� ���� ���� (static const: ���� ���� �� ����)
	UINT						m_nSwapChainBufferIndex;	// ���� �ĸ� ���� �ε���

	// ���� Ÿ��
	ID3D12Resource*				m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers]; // ����
	ID3D12DescriptorHeap*		m_pd3dRtvDescriptorHeap;						// ������
	UINT						m_nRtvDescriptorIncrementSize;					// ũ��

	// ����-���ٽ�
	ID3D12Resource*				m_pd3dDepthStencilBuffer;
	ID3D12DescriptorHeap*		m_pd3dDsvDescriptorHeap;
	UINT						m_nDsvDescriptorIncrementSize;

	// ���
	ID3D12CommandQueue*			m_pd3dCommandQueue;
	ID3D12CommandAllocator*		m_pd3dCommandAllocator;
	ID3D12GraphicsCommandList*	m_pd3dCommandList;

	// �׷��Ƚ� ���������� ����
	ID3D12PipelineState*		m_pd3dPipelineState;

	// �潺
	ID3D12Fence*				m_pd3dFence;
	UINT64						m_nFenceValue;
	HANDLE						m_hFenceEvent;

	// ����Ʈ, ���� �簢��
	D3D12_VIEWPORT				m_d3dViewport;
	D3D12_RECT					m_d3dScissorRect;



public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd); // �����ӿ�ũ �ʱ�ȭ

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



};

