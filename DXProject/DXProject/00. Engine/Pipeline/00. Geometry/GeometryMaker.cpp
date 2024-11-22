#include "pch.h"
#include "00. Engine/Pipeline/00. Geometry/GeometryMaker.h"

void GeometryMaker::CreateRectangle(Geometry<VertexTextureData>* geometry, float width, float height)
{	// 하단 중간이 중심이 되게 생성
	/*
	
			0		2

			1		3
			----+----

	*/

	float x = width / 2.f;
	float y = height / 2.f;

	vector<VertexTextureData> vertices;
	vertices.resize(4);
	vertices[0].position = Vec3(-x, height, 0.f);
	vertices[0].uv = Vec2(0.f, 0.f);

	vertices[1].position = Vec3(-x, -0.f, 0.f);
	vertices[1].uv = Vec2(0.f, 1.f);

	vertices[2].position = Vec3(x, height, 0.f);
	vertices[2].uv = Vec2(1.f, 0.f);

	vertices[3].position = Vec3(x, -0.f, 0.f);
	vertices[3].uv = Vec2(1.f, 1.f);
	geometry->SetVertices(vertices);

	vector<uint32> indices = { 0, 2, 3, 3, 1, 0 };
	geometry->SetIndices(indices);

	geometry->SetWidthAndHeight(width, height);
}

void GeometryMaker::CreateRectangleMidPointOnMidst(Geometry<VertexTextureData>* geometry, float width, float height)
{

	/*

			0		2
			----+----
			1		3

	*/
	float x = width / 2.f;
	float y = height / 2.f;

	vector<VertexTextureData> vertices;
	vertices.resize(4);
	vertices[0].position = Vec3(-x, y, 0.f);
	vertices[0].uv = Vec2(0.f, 0.f);

	vertices[1].position = Vec3(-x, -y, 0.f);
	vertices[1].uv = Vec2(0.f, 1.f);

	vertices[2].position = Vec3(x, y, 0.f);
	vertices[2].uv = Vec2(1.f, 0.f);

	vertices[3].position = Vec3(x, -y, 0.f);
	vertices[3].uv = Vec2(1.f, 1.f);
	geometry->SetVertices(vertices);

	vector<uint32> indices = { 0, 2, 3, 3, 1, 0 };
	geometry->SetIndices(indices);

	geometry->SetWidthAndHeight(width, height);
}

void GeometryMaker::CreateRectangleMidPointOnRight(Geometry<VertexTextureData>* geometry, float width, float height)
{
	/*

			0		2
			--------+
			1		3

	*/
	float x = width / 2.f;
	float y = height / 2.f;

	vector<VertexTextureData> vertices;
	vertices.resize(4);
	vertices[0].position = Vec3(-width, y, 0.f);
	vertices[0].uv = Vec2(0.f, 0.f);

	vertices[1].position = Vec3(-width, -y, 0.f);
	vertices[1].uv = Vec2(0.f, 1.f);

	vertices[2].position = Vec3(0.f, y, 0.f);
	vertices[2].uv = Vec2(1.f, 0.f);

	vertices[3].position = Vec3(0.f, -y, 0.f);
	vertices[3].uv = Vec2(1.f, 1.f);
	geometry->SetVertices(vertices);

	vector<uint32> indices = { 0, 2, 3, 3, 1, 0 };
	geometry->SetIndices(indices);

	geometry->SetWidthAndHeight(width, height);
}

void GeometryMaker::CreateRectangleMidPointOnLeft(Geometry<VertexTextureData>* geometry, float width, float height)
{
	/*

			0		2
			+--------
			1		3

	*/
	float x = width / 2.f;
	float y = height / 2.f;

	vector<VertexTextureData> vertices;
	vertices.resize(4);
	vertices[0].position = Vec3(0.f, y, 0.f);
	vertices[0].uv = Vec2(0.f, 0.f);

	vertices[1].position = Vec3(0.f, -y, 0.f);
	vertices[1].uv = Vec2(0.f, 1.f);

	vertices[2].position = Vec3(width, y, 0.f);
	vertices[2].uv = Vec2(1.f, 0.f);

	vertices[3].position = Vec3(width, -y, 0.f);
	vertices[3].uv = Vec2(1.f, 1.f);
	geometry->SetVertices(vertices);

	vector<uint32> indices = { 0, 2, 3, 3, 1, 0 };
	geometry->SetIndices(indices);

	geometry->SetWidthAndHeight(width, height);
}

void GeometryMaker::CreateRectangle(Geometry<VertexTextureData>* geometry)
{
	vector<VertexTextureData> vertices;
	vertices.resize(4);

	vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vertices[0].uv = Vec2(0.f, 1.f);
	vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vertices[1].uv = Vec2(0.f, 0.f);
	vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
	vertices[2].uv = Vec2(1.f, 1.f);
	vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
	vertices[3].uv = Vec2(1.f, 0.f);
	geometry->SetVertices(vertices);

	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(indices);
}
