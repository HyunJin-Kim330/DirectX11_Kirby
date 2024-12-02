#include "pch.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "00. Engine/Pipeline/00. Geometry/Geometry.h"
#include "00. Engine/Pipeline/00. Geometry/GeometryMaker.h"
#include "00. Engine/Pipeline/01. InputAssembler/InputLayout.h"
#include "00. Engine/Pipeline/02. VertexShader/ShaderBase.h"
#include "00. Engine/Manager/ResourceManager/Resource/Material.h"
#include "00. Engine/Manager/ResourceManager/Resource/Mesh.h"
#include "00. Engine/Manager/ResourceManager/Resource/ResourceBase.h"
#include "00. Engine/Manager/ResourceManager/ResourceManager.h"
#include "00. Engine/Manager/ResourceManager/Resource/Shader.h"
#include "00. Engine/Manager/ResourceManager/Resource/Texture.h"

ResourceManager::ResourceManager(ComPtr<ID3D11Device> _device) : device(_device)
{
	animXmlFileNames = 
	{
		"Resource/Xmls/normalKirby.xml",
		"Resource/Xmls/beamKirby.xml",
		"Resource/Xmls/boomerangKirby.xml",
		"Resource/Xmls/iceKirby.xml",
		"Resource/Xmls/sparkKirby.xml",
		"Resource/Xmls/things.xml",
		"Resource/Xmls/monsterAnim.xml",
		"Resource/Xmls/effect.xml",
		"Resource/Xmls/weapon.xml",
		"Resource/Xmls/backgroundAnim.xml"
	};
}

ResourceManager::~ResourceManager()
{
	for (auto& map : resources)
	{
		for (auto& pair : map)
		{
			delete pair.second;
		}
	}
}

void ResourceManager::Init()
{
	CreateDefaultShaderAndInputLayout();
	CreateDefaultTexture();
	CreateDefaultMesh();
	CreateDefaultMaterial();
	LoadAnimations();
}

void ResourceManager::AddResource(ResourceBase* resource, wstring name)
{
	uint8 index = static_cast<uint8>(resource->GetType());
	resources[index][name] = resource;
}

void ResourceManager::CreateDefaultShaderAndInputLayout()
{
	{
		auto vertexShader = new VertexShader(device);
		vertexShader->Create(L"10. Shader/Default.hlsl", "VS", "vs_5_0");
		auto inputLayout = new InputLayout(device);
		inputLayout->Create(VertexTextureData::descs, vertexShader->GetBlob());
		auto pixelShader = new PixelShader(device);
		pixelShader->Create(L"10. Shader/Default.hlsl", "PS", "ps_5_0");

		Shader* shader = new Shader();
		shader->SetName(L"Default");
		shader->vertexShader = vertexShader;
		shader->inputLayout = inputLayout;
		shader->pixelShader = pixelShader;

		AddResource(shader, shader->GetName());
	}

	{
		auto vertexShader = new VertexShader(device);
		vertexShader->Create(L"10. Shader/collider.hlsl", "VS", "vs_5_0");
		auto inputLayout = new InputLayout(device);
		inputLayout->Create(VertexTextureData::descs, vertexShader->GetBlob());
		auto pixelShader = new PixelShader(device);
		pixelShader->Create(L"10. Shader/collider.hlsl", "PS", "ps_5_0");

		Shader* shader = new Shader();
		shader->SetName(L"Collider");
		shader->vertexShader = vertexShader;
		shader->inputLayout = inputLayout;
		shader->pixelShader = pixelShader;

		AddResource(shader, shader->GetName());
	}
}

void ResourceManager::CreateDefaultMaterial()
{
	{
		Material* material = new Material();
		material->SetName(L"Default");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"Kirby"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"1-1");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"Level1-1Foreground"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"1-3");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"Level1-3ForeGround"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"1-5");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"boss"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"abilityInfo");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"NormalIcon"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"kirbyLife");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"life2"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"monsterBlood");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"BossHP"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"monsterBloodBorder");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"HPBorder"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"kirbyBlood");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"KirbyBlood"));
		AddResource(material, material->GetName());
	}
	{
		Material* material = new Material();
		material->SetName(L"kirbyBloodBorder");
		material->SetShader(Get<Shader>(L"Default"));
		material->SetTexture(Get<Texture>(L"KirbyBloodBorder"));
		AddResource(material, material->GetName());
	}
}

void ResourceManager::CreateDefaultTexture()
{
	tinyxml2::XMLDocument doc;

	XMLError error = doc.LoadFile("Resource/Xmls/texture.xml");
	assert(error == XMLError::XML_SUCCESS);

	XmlElement* root = doc.FirstChildElement();

	XmlElement* tex = root->FirstChildElement();
	for (; tex != nullptr; tex = tex->NextSiblingElement())
	{
		string nameStr = tex->Attribute("Name");
		wstring name = wstring(nameStr.begin(), nameStr.end());

		string pathStr = tex->Attribute("Path");
		wstring path = wstring(pathStr.begin(), pathStr.end());

		auto texture = new Texture(device);
		texture->SetName(name);
		texture->Create(path);

		AddResource(texture, texture->GetName());
	}
}

void ResourceManager::CreateDefaultMesh()
{
	tinyxml2::XMLDocument doc;

	XMLError error = doc.LoadFile("Resource/Xmls/mesh.xml");
	assert(error == XMLError::XML_SUCCESS);

	XmlElement* root = doc.FirstChildElement();

	XmlElement* mesh = root->FirstChildElement();
	for (; mesh != nullptr; mesh = mesh->NextSiblingElement())
	{
		string nameStr = mesh->Attribute("Name");
		wstring name = wstring(nameStr.begin(), nameStr.end());

		string midPoint = mesh->Attribute("MidPoint");

		float width = mesh->FloatAttribute("Width");
		float height = mesh->FloatAttribute("Height");

		Geometry<VertexTextureData>* geometry = new Geometry<VertexTextureData>();
		
		if (midPoint == "Midst")
		{
			GeometryMaker::CreateRectangleMidPointOnMidst(geometry, width, height);
		}
		else if (midPoint == "Bottom")
		{
			GeometryMaker::CreateRectangle(geometry, width, height);
		}
		else if (midPoint == "Left")
		{
			GeometryMaker::CreateRectangleMidPointOnLeft(geometry, width, height);
		}
		else if (midPoint == "Right")
		{
			GeometryMaker::CreateRectangleMidPointOnRight(geometry, width, height);
		}
		
		Mesh* mesh = new Mesh(device, geometry);
		mesh->SetName(name);
		AddResource(mesh, mesh->GetName());
	}
}

void ResourceManager::LoadAnimations()
{
	tinyxml2::XMLDocument doc;

	for (string& fileName : animXmlFileNames)  // fileName의 수 만큼 돌면서 애니메이션 생성
	{
		XMLError error = doc.LoadFile(fileName.c_str());
		assert(error == XMLError::XML_SUCCESS);

		XmlElement* root = doc.FirstChildElement();
		
		XmlElement* anim = root->FirstChildElement();
		for (; anim != nullptr; anim = anim->NextSiblingElement())
		{
			string nameStr = anim->Attribute("Name");
			wstring name = wstring(nameStr.begin(), nameStr.end());
			bool loop = anim->BoolAttribute("Loop");
			string fileName = anim->Attribute("TextureName");
			wstring textureName = wstring(fileName.begin(), fileName.end());
			Texture* texture = Get<Texture>(textureName);
			string gatherName = anim->Attribute("Tag");
			int num = anim->IntAttribute("Num");

			Animation* animation = new Animation();
			animation->SetName(name);
			animation->SetLoop(loop);
			animation->SetTexture(texture);

			XmlElement* node = anim->FirstChildElement();
			for (; node != nullptr; node = node->NextSiblingElement())
			{
				Frame frame;

				frame.offset.x = node->FloatAttribute("X");
				frame.offset.y = node->FloatAttribute("Y");
				frame.size.x = node->FloatAttribute("Width");
				frame.size.y = node->FloatAttribute("Height");
				frame.duration = node->FloatAttribute("Duration");

				animation->AddFrame(frame);
			}
			AddResource(animation, animation->GetName());
			if (gatherName != "")
			{
				animationGater[gatherName][num] = animation;
			}
		}
	}
}

map<int, Animation*> ResourceManager::GetAnimationGater(string gatherName)
{
	return animationGater[gatherName];
}
