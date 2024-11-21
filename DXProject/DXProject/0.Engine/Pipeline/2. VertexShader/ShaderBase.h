#pragma once

class ShaderBase
{
public:
	ShaderBase(ComPtr<ID3D11Device> _device);
	virtual ~ShaderBase();

	virtual void Create(const wstring& path, const string& name, const string& version) abstract;
	ComPtr<ID3DBlob> GetBlob() { return blob; }

protected:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version);

protected:
	wstring path;
	string name;
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11Device> device;
};

class VertexShader : public ShaderBase
{
	using Super = ShaderBase;
public:
	VertexShader(ComPtr<ID3D11Device> _device);
	~VertexShader();

	virtual void Create(const wstring& path, const string& name, const string& version) override;
	ComPtr<ID3D11VertexShader> GetVertexShader() { return vertexShader; }

private:
	ComPtr<ID3D11VertexShader> vertexShader;
};

class PixelShader : public ShaderBase
{
	using Super = ShaderBase;
public:
	PixelShader(ComPtr<ID3D11Device> _device);
	~PixelShader();

	virtual void Create(const wstring& path, const string& name, const string& version) override;
	ComPtr<ID3D11PixelShader> GetPixelShader() { return pixelShader; }

private:
	ComPtr<ID3D11PixelShader> pixelShader;
};
