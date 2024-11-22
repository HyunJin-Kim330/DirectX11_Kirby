#pragma once
#include "00. Engine/Pipeline/02. VertexShader/ConstantBuffer.h"

class BlendState;
class IndexBuffer;
class InputLayout;
class PixelShader;
class SamplerState;
class Texture;
class VertexBuffer;
class VertexShader;

/*
* 작은 프로젝트이고, 쉐이더 파일을 거의 하나만 공용적으로 사용할 것이기 때문에
* 이렇게 선언함
*/ 
struct PipelineInfo
{
	InputLayout* inputLayout;
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	BlendState* blendState;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> _deviceContext);
	~Pipeline();

	void updatePipeline(PipelineInfo info);
	void SetVertexBuffer(VertexBuffer* vertexBuffer);
	void SetIndexBuffer(IndexBuffer* indexBuffer);

	template<typename T>
	void SetConstantBuffer(uint32 slot, ConstantBuffer<T>* buffer)
	{
		deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetConstantBuffer().GetAddressOf());
	}
	
	void SetTexture(uint32 slot, Texture* texture);
	void SetSamplerState(uint32 slot, SamplerState* samplerState);
	void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation);
	void Draw(uint32 verteicesCount);

private:
	ComPtr<ID3D11DeviceContext> deviceContext;

};

