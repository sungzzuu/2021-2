#pragma once

// 정점으로 이루어진 물체
class Mesh
{
public:
	void Init(vector<Vertex>& vertexBuffer, const vector<uint32>& buffer);
	void Render();

	void SetTransform(const Transform& t) { m_transform = t; }
	void SetTexture(shared_ptr<class Texture> tex) { m_tex = tex; }

private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);


private:
	ComPtr<ID3D12Resource>		m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView = {};
	uint32 m_vertexCount = 0;

	ComPtr<ID3D12Resource>		m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW		m_indexBufferView = {};
	uint32 m_indexCount = 0;

	Transform m_transform = {};
	shared_ptr<class Texture> m_tex = {};
};

