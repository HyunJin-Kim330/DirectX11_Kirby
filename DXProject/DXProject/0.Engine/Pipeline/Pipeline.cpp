#include "pch.h"
#include "Pipeline.h"

#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "ShaderBase.h"
#include "SamplerState.h"

Pipeline::Pipeline(ComPtr<ID3D11DeviceContext> _deviceContext)
	: deviceContext(_deviceContext)
{

}

Pipeline::~Pipeline()
{

}

void Pipeline::updatePipeline(PipelineInfo info)
{
	// IA
	deviceContext->IASetInputLayout(info.inputLayout->GetComPtr().Get());
	deviceContext->IASetPrimitiveTopology(info.topology);

	// VS
	if (info.vertexShader)
		deviceContext->VSSetShader(info.vertexShader->GetVertexShader().Get(), nullptr, 0);

	// RS

	// PS
	if (info.pixelShader)
		deviceContext->PSSetShader(info.pixelShader->GetPixelShader().Get(), nullptr, 0);

	// OM
	if (info.blendState)
		deviceContext->OMSetBlendState(info.blendState->GetComPtr().Get(), nullptr, 0xFFFFFFFFu);
}

void Pipeline::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
	uint32 stride = vertexBuffer->GetStride();
	uint32 offset = vertexBuffer->GetOffset();
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(IndexBuffer* indexBuffer)
{
	deviceContext->IASetIndexBuffer(indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::SetTexture(uint32 slot, Texture* texture)
{
	deviceContext->PSSetShaderResources(slot, 1, texture->GetShaderResourceView().GetAddressOf());
}

void Pipeline::SetSamplerState(uint32 slot, SamplerState* samplerState)
{
	deviceContext->PSSetSamplers(0, 1, samplerState->GetComPtr().GetAddressOf());
}

void Pipeline::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation)
{
	deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Pipeline::Draw(uint32 verteicesCount)
{
	deviceContext->Draw(verteicesCount, 0);
}