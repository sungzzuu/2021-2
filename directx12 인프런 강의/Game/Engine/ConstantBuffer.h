#pragma once
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	D3D12_CPU_DESCRIPTOR_HANDLE PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	ComPtr<ID3D12Resource>	m_cbvBuffer;	// ���� ������
	BYTE*					m_mappedBuffer = nullptr;
	uint32					m_elementSize = 0;
	uint32					m_elementCount = 0;

	ComPtr<ID3D12DescriptorHeap>		m_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			m_cpuHandleBegin = {}; // �����ڵ��ּ�
	uint32								m_handleIncrementSize = 0; // ��ĭ�� �����ϴ���. �ڵ鰣�� ����

	uint32					m_currentIndex = 0; // 0~end, �ٽ� 0

};

