#pragma once
class Engine
{
public:
	void Init(const WindowInfo& window);
	void Render();

	void ResizeWindow(int32 width, int32 height);

private:
	WindowInfo		m_window;
	D3D12_VIEWPORT	m_viewport = {};
	D3D12_RECT		m_sissorRect = {};

	shared_ptr<class Device> m_device;
	shared_ptr<class CommandQueue> m_cmdQueue;
	shared_ptr<class SwapChain> m_swapChain;
	shared_ptr<class DescriptorHeap> _descHeap;

};

