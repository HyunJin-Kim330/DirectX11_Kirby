#pragma once
#include "ResourceBase.h"
#include "ShaderBase.h"
#include "InputLayout.h"
#include "Texture.h"
#include "Shader.h"

class Material : public ResourceBase
{
	using Super = ResourceBase;
public:
	Material();
	~Material();

	Shader* GetShader() { return shader; }
	void SetShader(Shader* _shader) { shader = _shader; }
	Texture* GetTexture() { return texture; }
	void SetTexture(Texture* _texture) { texture = _texture; }

private:
	Shader* shader;
	Texture* texture;
};

