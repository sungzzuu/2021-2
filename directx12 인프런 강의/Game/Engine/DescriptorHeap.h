#pragma once


// [��ȼ�]
// ���ָ� �ñ� �� �̷� ���� �������� ���� �Ѱ���� �ϴµ�,
// �ƹ� ���·γ� ��û�ϸ� �� �˾ƸԴ´�
// - ���� ���ҽ��� � �뵵�� ����ϴ��� �Ĳ��ϰ� ��� �Ѱ��ִ� �뵵
class DescriptorHeap // View
{
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTV(int32 idx) { return m_rtvHandle[idx]; }

	D3D12_CPU_DESCRIPTOR_HANDLE		GetBackBufferView();

private:
	ComPtr<ID3D12DescriptorHeap>	m_rtvHeap;
	uint32							m_rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	shared_ptr<class SwapChain>		m_swapChain;
};