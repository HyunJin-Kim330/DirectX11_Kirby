#pragma once

enum class ResourceType : uint8
{
	None = -1,
	Mesh,
	Texture,
	Shader,
	Material,
	Animation,

	End
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<uint8>(ResourceType::End)
};

class ResourceBase : public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(ResourceType _type);
	virtual ~ResourceBase();

	ResourceType GetType() { return type; }

	void SetName(const wstring& _name) { name = _name; }
	const wstring& GetName() { return name; }
	uint32 GetID() { return _id; }

protected:
	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

protected:
	ResourceType type = ResourceType::None;
	wstring name;
	wstring _path;
	uint32 _id = 0;
};

