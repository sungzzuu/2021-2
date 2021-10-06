#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"

class Engine
{
public:
	void Init(const WindowInfo& window);
	void Render();

public:
	shared_ptr<Device> GetDevice() { return m_device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return m_cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return m_swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return m_rootSignature; }

public:
	void RenderBegin();
	void RenderEnd();
	void ResizeWindow(int32 width, int32 height);

private:
	WindowInfo		m_window;
	D3D12_VIEWPORT	m_viewport = {};
	D3D12_RECT		m_scissorRect = {};

	shared_ptr<Device> m_device; 
	shared_ptr<CommandQueue> m_cmdQueue;
	shared_ptr<SwapChain> m_swapChain;
	shared_ptr<RootSignature> m_rootSignature;

};

 