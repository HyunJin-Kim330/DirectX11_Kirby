#pragma once
#include "ResourceBase.h"

class InputLayout;
class PixelShader;
class VertexShader;

class Shader : public ResourceBase
{
	using Super = ResourceBase;
public:
	Shader();
	virtual ~Shader();

	InputLayout* GetInputLayout() { return inputLayout; }
	VertexShader* GetVertexShader() { return vertexShader; }
	PixelShader* GetPixelShader() { return pixelShader; }

private:
	friend class ResourceManager;

	InputLayout* inputLayout;
	VertexShader* vertexShader;
	PixelShader* pixelShader;
};

