#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    return false;
}

void CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{

}

void CScene::CreateGraphicsGraphicsPipelineState(ID3D12Device* pd3dDevice)
{
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice)
{
}

void CScene::ReleaseObjects()
{
}

bool CScene::ProcessInput()
{
    return false;
}

void CScene::AnimateObjects(float fTimeElapsed)
{
}

void CScene::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
}
