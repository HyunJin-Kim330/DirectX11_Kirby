#include "pch.h"
#include "00. Engine/Pipeline/01. InputAssembler/InputLayout.h"

InputLayout::InputLayout(ComPtr<ID3D11Device> _device)
	: device(_device)
{

}

InputLayout::~InputLayout()
{

}


void InputLayout::Create(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, ComPtr<ID3DBlob> blob)
{
	const int32 count = static_cast<int32>(descs.size());
	device->CreateInputLayout(descs.data(), count, blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout.GetAddressOf());
}
