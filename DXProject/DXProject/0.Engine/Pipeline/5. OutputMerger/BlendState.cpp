#include "pch.h"
#include "BlendState.h"

BlendState::BlendState(ComPtr<ID3D11Device> device) : _device(device)
{

}

BlendState::~BlendState()
{

}

void BlendState::Create(D3D11_RENDER_TARGET_BLEND_DESC blendDesc /*= { true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL }*/, float factor /*= 0.f*/)
{
	_blendFactor = factor;
	
	blendDesc.BlendEnable = TRUE;
	blendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0] = blendDesc;

	HRESULT hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}
