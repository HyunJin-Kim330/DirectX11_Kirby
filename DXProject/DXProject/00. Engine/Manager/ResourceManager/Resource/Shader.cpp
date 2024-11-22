#include "pch.h"
#include "00. Engine/Manager/ResourceManager/Resource/Shader.h"

Shader::Shader() : Super(ResourceType::Shader)
{

}

Shader::~Shader()
{
	delete inputLayout;
	delete vertexShader;
	delete pixelShader;
}
