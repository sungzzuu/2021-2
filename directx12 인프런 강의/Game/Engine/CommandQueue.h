#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{

public:
	~CommandQueue();
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

public:
	ComPtr<ID3D12CommandQueue>			GetCmdQueue() { return m_cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList>	GetCmdList() { return m_cmdList; }
	ComPtr<ID3D12GraphicsCommandList>	GetResourceCmdList() { return	m_resCmdList; }


private:
	// CommandQueue : DX12에 등장
	// 외주를 요청할 때, 하나씩 요청하면 비효율적
	// [외주 목록]에 일감을 차곡차곡 기록했다가 한 방에 요청하는 것
	ComPtr<ID3D12CommandQueue>			m_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		m_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	m_cmdList;

	ComPtr<ID3D12CommandAllocator>		m_resCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	m_resCmdList;

	// Fence : 울타리(?)
	// CPU / GPU 동기화를 위한 간단한 도구
	ComPtr<ID3D12Fence>					m_fence;
	uint32								m_fenceValue = 0;
	HANDLE								m_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>			m_swapChain;
};

