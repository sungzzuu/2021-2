#include "pch.h"
#include "Engine.h"

void Engine::Init(const WindowInfo& window)
{
	m_window = window;
	ResizeWindow(window.width, window.height);

	// 그려질 화면 크기를 설정
	m_viewport = { 0, 0, static_cast<FLOAT>(window.width), static_cast<FLOAT>(window.height), 0.f, 1.f };
	m_sissorRect = CD3DX12_RECT(0, 0, window.width, window.height);


}

void Engine::Render()
{
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	m_window.width = width;
	m_window.height = height;

	RECT rect = { 0,0,width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(m_window.hWnd, 0, 100, 100, width, height, 0);
}
