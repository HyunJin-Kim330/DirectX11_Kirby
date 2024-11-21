#pragma once

struct VertexTextureData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexData
{
	Vec3 position = { 0, 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

template<typename T>
class Geometry
{
public:
	Geometry() {}
	~Geometry() {}

	vector<T>& GetVertices() { return vertices; }
	vector<uint32>& GetIndices() { return indices; }
	float GetWidth() const { return width; }
	float GetHeight() const { return height; }

	void SetVertices(const vector<T>& _vertices) { vertices = _vertices; }
	void SetIndices(const vector<uint32>& _indices) { indices = _indices; }
	void SetWidthAndHeight(float _width, float _height) { width = _width; height = _height; }

private:
	vector<T> vertices;
	vector<uint32> indices;
	float width;
	float height;
};

