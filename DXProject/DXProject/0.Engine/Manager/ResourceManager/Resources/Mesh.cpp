#include "pch.h"
#include "Mesh.h"
#include "Geometry.h"

Mesh::Mesh(ComPtr<ID3D11Device> _device, Geometry<VertexTextureData>* _geometry) : Super(ResourceType::Mesh), device(_device)
{
	geometry = _geometry;

	vertexBuffer = new VertexBuffer(device);
	vertexBuffer->Create(_geometry->GetVertices());

	indexBuffer = new IndexBuffer(device);
	indexBuffer->Create(_geometry->GetIndices());
}

Mesh::~Mesh()
{
	delete geometry;
	delete vertexBuffer;
	delete indexBuffer;
}

void Mesh::AdjustMeshWidthForRatio(float ratio)
{
	float width = geometry->GetWidth();
	vector<VertexTextureData> vertices = geometry->GetVertices();

	vertices[2].position.x = width * ratio;
	vertices[3].position.x = width * ratio;

	vertexBuffer->Create(vertices);
}