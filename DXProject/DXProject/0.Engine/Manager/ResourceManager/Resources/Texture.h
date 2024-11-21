#pragma once
#include "ResourceBase.h"

class Texture : public ResourceBase
{
	using Super = ResourceBase;
public:
	Texture(ComPtr<ID3D11Device> _device);
	~Texture();

	void Create(const wstring& path);
	Vec2 GetSize() { return _size; }
	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return shaderResourveView; }

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11ShaderResourceView> shaderResourveView;
	Vec2 _size = { 0.f, 0.f };
};

