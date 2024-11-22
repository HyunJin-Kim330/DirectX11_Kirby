#pragma once
#include "00. Engine/Manager/ResourceManager/Resource/ResourceBase.h"

class Animation;
class Material;
class Mesh;
class Shader;
class Texture;

class ResourceManager
{
public:
	ResourceManager(ComPtr<ID3D11Device> _device);
	~ResourceManager();

	void Init();

public:
	void AddResource(ResourceBase* resource, wstring name);
	void CreateDefaultTexture();
	void CreateDefaultMesh();
	void CreateDefaultShaderAndInputLayout();
	void CreateDefaultMaterial();

	template<typename T>
	T* Get(const wstring& key);

	template<typename T>
	ResourceType GetResourceType();

	map<int, Animation*> GetAnimationGater(string gatherName);

private:
	ComPtr<ID3D11Device> device;
	array<map<wstring, ResourceBase*>, RESOURCE_TYPE_COUNT> resources;
	map<string, map<int, Animation*>> animationGater;
	vector<string> animXmlFileNames;

private:
	void LoadAnimations();

};

template<typename T>
T* ResourceManager::Get(const wstring& key)
{
	ResourceType resourceType = GetResourceType<T>();
	map<wstring, ResourceBase*> keyObjMap = resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_cast<T*>(findIt->second);

	return nullptr;
}

template<typename T>
ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Material>)
		return ResourceType::Material;
	if (std::is_same_v<T, Animation>)
		return ResourceType::Animation;

	assert(false);
	return ResourceType::None;
}
