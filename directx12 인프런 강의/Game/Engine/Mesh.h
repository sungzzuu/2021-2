#pragma once

// 정점으로 이루어진 물체
class Mesh
{
public:
	void Init(vector<Vertex>& vec);
	void Render();
private:
	ComPtr<ID3D12Resource>		m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView = {};
	uint32 m_vertexCount = 0;
};

