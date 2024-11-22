#pragma once
#include "00. Engine/Manager/ResourceManager/Resource/ResourceBase.h"
#include "00. Engine/Pipeline/02. VertexShader/ShaderBase.h"
#include "00. Engine/Pipeline/01. InputAssembler/InputLayout.h"
#include "00. Engine/Manager/ResourceManager/Resource/Texture.h"
#include "00. Engine/Manager/ResourceManager/Resource/Shader.h"

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

