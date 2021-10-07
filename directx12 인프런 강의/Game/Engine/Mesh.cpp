#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec, const vector<uint32>& buffer)
{
	CreateVertexBuffer(vec);
	CreateIndexBuffer(buffer);

}

void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &m_vertexBufferView); // Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&m_indexBufferView);

	// TODO
	// 1) Buffer에다가 데이터 세팅
	// 2) TableDescHeap에다가 CBV 전달
	// 3) 모두 세팅이 끝났으면 TableDescHeap 커밋
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &m_transform, sizeof(m_transform));
		GEngine->GetTableDescriptorHeap()->SetCBV(handle, CBV_REGISTER::b0);
		GEngine->GetTableDescriptorHeap()->SetSRV(m_tex->GetCpuHandle(), SRV_REGISTER::t0);

	}

	GEngine->GetTableDescriptorHeap()->CommitTable();

	//GEngine->GetCB()->PushData(0, &m_transform, sizeof(m_transform));
	//GEngine->GetCB()->PushData(1, &m_transform, sizeof(m_transform));

	//CMD_LIST->DrawInstanced(m_vertexCount, 1, 0, 0);
	CMD_LIST->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	m_vertexCount = static_cast<uint32>(buffer.size()); // 정점 목록 저장
	uint32 bufferSize = m_vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// 타입만 지정. 가장 간단한 형태
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	m_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize);
	m_vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	m_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	m_indexCount = static_cast<uint32>(buffer.size()); // 정점 목록 저장
	uint32 bufferSize = m_indexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// 타입만 지정. 가장 간단한 형태
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	m_indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	m_indexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R32_UINT; // 정점 1개 크기
	m_indexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}
