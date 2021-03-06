#include "pch.h"
#include "Engine.h"

void Engine::Init(const WindowInfo& window)
{
	m_window = window;
	ResizeWindow(window.width, window.height);

	// 그려질 화면 크기를 설정
	m_viewport = { 0, 0, static_cast<FLOAT>(window.width), static_cast<FLOAT>(window.height), 0.f, 1.f };
	m_scissorRect = CD3DX12_RECT(0, 0, window.width, window.height);

	m_device = make_shared<Device>();
	m_cmdQueue = make_shared<CommandQueue>();
	m_swapChain = make_shared<SwapChain>();
	m_rootSignature = make_shared<RootSignature>();
	m_cb = make_shared<ConstantBuffer>();
	m_tableDescriptorHeap = make_shared<TableDescriptorHeap>();

	m_device->Init();
	m_cmdQueue->Init(m_device->GetDevice(), m_swapChain);
	m_swapChain->Init(window, m_device->GetDevice(), m_device->GetDXGI(), m_cmdQueue->GetCmdQueue());
	m_rootSignature->Init();
	m_cb->Init(sizeof(Transform), 256);
	m_tableDescriptorHeap->Init(256);
}

void Engine::Render()
{
	RenderBegin();

	// TODO: 여기에 물체 추가

	RenderEnd();

}

void Engine::RenderBegin()
{
	m_cmdQueue->RenderBegin(&m_viewport, &m_scissorRect);
}

void Engine::RenderEnd()
{
	m_cmdQueue->RenderEnd();
}


void Engine::ResizeWindow(int32 width, int32 height)
{
	m_window.width = width;
	m_window.height = height;

	RECT rect = { 0,0,width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(m_window.hWnd, 0, 100, 100, width, height, 0);
}
