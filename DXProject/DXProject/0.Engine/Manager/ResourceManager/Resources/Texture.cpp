#include "pch.h"
#include "Texture.h"

Texture::Texture(ComPtr<ID3D11Device> _device)
	: Super(ResourceType::Texture), device(_device)
{

}

Texture::~Texture()
{

}

void Texture::Create(const wstring& path)
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(device.Get(), img.GetImages(), img.GetImageCount(), md, shaderResourveView.GetAddressOf());
	CHECK(hr);

	_size.x = md.width;
	_size.y = md.height;
}
