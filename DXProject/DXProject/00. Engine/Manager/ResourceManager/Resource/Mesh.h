#pragma once
#include "00. Engine/Pipeline/00. Geometry/Geometry.h"
#include "00. Engine/Pipeline/01. InputAssembler/IndexBuffer.h"
#include "00. Engine/Pipeline/01. InputAssembler/VertexBuffer.h"

class Mesh : public ResourceBase
{
	using Super = ResourceBase;
public:
	Mesh(ComPtr<ID3D11Device> _device, Geometry<VertexTextureData>* geometry);
	~Mesh();

	Geometry<VertexTextureData>* GetGeometry() { return geometry; }
	VertexBuffer* GetVertexBuffer() { return vertexBuffer; }
	IndexBuffer* GetIndexBuffer() { return indexBuffer; }
	float GetWidth() { return geometry->GetWidth(); }
	float GetHeight() { return geometry->GetHeight(); }

	void AdjustMeshWidthForRatio(float ratio);

private:
	ComPtr<ID3D11Device> device;
	Geometry<VertexTextureData>* geometry;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	float meshWidth;
	float meshHeight;

};

