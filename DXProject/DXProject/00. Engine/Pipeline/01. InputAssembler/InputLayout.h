#pragma once

class InputLayout
{
public:
	InputLayout(ComPtr<ID3D11Device> _device);
	~InputLayout();

	ComPtr<ID3D11InputLayout> GetComPtr() { return inputLayout; }

	void Create(const vector< D3D11_INPUT_ELEMENT_DESC>& descs, ComPtr<ID3DBlob> blob);

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11InputLayout> inputLayout;
};

