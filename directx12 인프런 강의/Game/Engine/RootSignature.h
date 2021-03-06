#pragma once

// [계약서 / 결재]

//  CPU [	]	GPU [	]

//	한국 [	]	벳남 [	]

class RootSignature
{
public:
	void Init();
	ComPtr<ID3D12RootSignature> GetSignature() { return m_signature; }

private:
	void CreateSamplerDesc();
	void CreateRootSignature();

private:
	ComPtr<ID3D12RootSignature> m_signature;
	D3D12_STATIC_SAMPLER_DESC	m_samplerDesc;

};

