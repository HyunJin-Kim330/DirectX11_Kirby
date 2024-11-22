#include "pch.h"
#include "00. Engine/Pipeline/02. VertexShader/ShaderBase.h"

ShaderBase::ShaderBase(ComPtr<ID3D11Device> _device) : device(_device)
{

}

ShaderBase::~ShaderBase()
{

}

void ShaderBase::LoadShaderFromFile(const wstring& _path, const string& _name, const string& _version)
{
	path = _path;
	name = _name;

	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		_version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	CHECK(hr);
}

VertexShader::VertexShader(ComPtr<ID3D11Device> device) : Super(device)
{

}

VertexShader::~VertexShader()
{

}

void VertexShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);
	HRESULT hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
	CHECK(hr);
}

PixelShader::PixelShader(ComPtr<ID3D11Device> device) : Super(device)
{

}

PixelShader::~PixelShader()
{

}

void PixelShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);
	HRESULT hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
	CHECK(hr);
}