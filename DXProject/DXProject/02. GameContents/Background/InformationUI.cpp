#include "pch.h"
#include "00. Engine/GameObject/EmptyObject.h"
#include "02. GameContents/Background/InformationUI.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "02. GameContents/Player/Kirby/Kirby.h"

InformationUI::InformationUI()
{
	CreateAbilityInfo();
	CreateLifeInfo();
	CreateKirbyBlood();
	CreateMonsterBlood();
}

InformationUI::~InformationUI()
{

}

void InformationUI::UpdateObject()
{
	MeshRenderer* meshRenderer = abilityInfo->GetMeshRenderer();
	meshRenderer->SetTexture(GAME->GetResourceManager()->Get<Texture>(abilityInfoTexture));

	meshRenderer = lifeInfo->GetMeshRenderer();
	meshRenderer->SetTexture(GAME->GetResourceManager()->Get<Texture>(kirbyLifeInfoTexture));

	FollowCamera();
}

void InformationUI::CreateAbilityInfo()
{
	abilityInfo = new EmptyObject();
	AddChild(abilityInfo);
	abilityInfo->SetParent(this);

	//abilityInfo->GetTransform()->SetPosition(Vec3{ -5.9f, -3.5f, 0.f });
	abilityInfo->SetInitPosition(Vec3{ -5.9f, -3.5f, 0.f });

	MeshRenderer* meshRenderer = new MeshRenderer();
	abilityInfo->AddComponent(meshRenderer);

	meshRenderer->SetRenderingOrder(15);
	auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"Midst2x2");
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"abilityInfo");
	meshRenderer->SetMaterial(material);
}

void InformationUI::CreateLifeInfo()
{
	lifeInfo = new EmptyObject();
	AddChild(lifeInfo);
	lifeInfo->SetParent(this);

	//lifeInfo->GetTransform()->SetPosition(Vec3{ -3.4f, -3.2f, 0.f });
	lifeInfo->SetInitPosition(Vec3{ -3.4f, -3.2f, 0.f });

	MeshRenderer* meshRenderer = new MeshRenderer();
	lifeInfo->AddComponent(meshRenderer);

	meshRenderer->SetRenderingOrder(15);
	auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"Midst2.17x0.7");
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"kirbyLife");
	meshRenderer->SetMaterial(material);
}

void InformationUI::CreateKirbyBlood()
{
	{
		kirbyBloodBorder = new EmptyObject();

		AddChild(kirbyBloodBorder);
		kirbyBloodBorder->SetParent(this);
		kirbyBloodBorder->SetInitPosition(Vec3{ -4.8f, -4.f, 0.f });


		MeshRenderer* meshRenderer = new MeshRenderer();
		kirbyBloodBorder->AddComponent(meshRenderer);
		meshRenderer->SetRenderingOrder(14);

		auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"KirbyBloodBorder");
		meshRenderer->SetMesh(mesh);
		auto material = GGameManager->GetResourceManager()->Get<Material>(L"kirbyBloodBorder");
		meshRenderer->SetMaterial(material);
	}
	{
		kirbyBloodInfo = new EmptyObject();
		AddChild(kirbyBloodInfo);
		kirbyBloodInfo->SetParent(this);

		//kirbyBloodInfo->GetTransform()->SetPosition(Vec3{ -4.55f, -4.f, 0.f });
		kirbyBloodInfo->SetInitPosition(Vec3{ -4.55f, -4.f, 0.f });

		MeshRenderer* meshRenderer = new MeshRenderer();
		kirbyBloodInfo->AddComponent(meshRenderer);
		meshRenderer->SetRenderingOrder(15);

		auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"KirbyBlood");
		meshRenderer->SetMesh(mesh);
		auto material = GGameManager->GetResourceManager()->Get<Material>(L"kirbyBlood");
		meshRenderer->SetMaterial(material);
	}
}

void InformationUI::CreateMonsterBlood()
{
	{
		monsterBloodInfo = new EmptyObject();
		AddChild(monsterBloodInfo);
		monsterBloodInfo->SetParent(this);

		//monsterBloodInfo->GetTransform()->SetPosition(Vec3{ 2.8f, -4.f, 0.f });
		monsterBloodInfo->SetInitPosition(Vec3{ 2.8f, -4.f, 0.f });

		MeshRenderer* meshRenderer = new MeshRenderer();
		monsterBloodInfo->AddComponent(meshRenderer);
		meshRenderer->SetRenderingOrder(15);

		auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"MonsterBlood");
		meshRenderer->SetMesh(mesh);
		auto material = GGameManager->GetResourceManager()->Get<Material>(L"monsterBlood");
		meshRenderer->SetMaterial(material);
		meshRenderer->InactiveMeshRenderer();
	}
	{
		monsterBloodBorder = new EmptyObject();
		AddChild(monsterBloodBorder);
		monsterBloodBorder->SetParent(this);
		//monsterBloodBorder->GetTransform()->SetPosition(Vec3{ 2.45f, -4.f, 0.f });
		monsterBloodBorder->SetInitPosition(Vec3{ 2.45f, -4.f, 0.f });

		MeshRenderer* meshRenderer = new MeshRenderer();
		monsterBloodBorder->AddComponent(meshRenderer);
		meshRenderer->SetRenderingOrder(14);

		auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"MonsterBloodBorder");
		meshRenderer->SetMesh(mesh);
		auto material = GGameManager->GetResourceManager()->Get<Material>(L"monsterBloodBorder");
		meshRenderer->SetMaterial(material);
		meshRenderer->InactiveMeshRenderer();
	}
}

void InformationUI::FollowCamera()
{
	GetTransform()->SetPosition(GAME->GetSceneManager()->GetCameraPosition());
}

void InformationUI::UpdateInfo(const Kirby* kirby, float _monsterRemainBloodRatio)
{
	abilityInfoTexture = kirby->GetAbilityInfoTexture();
	
	int kirbyLife = kirby->GetLife();

	switch (kirbyLife)
	{
	case 0:
		kirbyLifeInfoTexture = L"life0";
		break;
	case 1:
		kirbyLifeInfoTexture = L"life1";
		break;
	case 2:
		kirbyLifeInfoTexture = L"life2";
		break;
	default:
		kirbyLifeInfoTexture = L"life0";
		break;
	}

	kirbyBloodInfo->GetMeshRenderer()->GetMesh()->AdjustMeshWidthForRatio(kirby->GetBlood() / 100.f);

	if (_monsterRemainBloodRatio != -1.0f)
	{
		monsterBloodDrawTimeCalc = 0.f;
		monsterRemainBloodRatio = _monsterRemainBloodRatio;
		monsterBloodInfo->GetMeshRenderer()->ActiveMeshRenderer();
		monsterBloodBorder->GetMeshRenderer()->ActiveMeshRenderer();
	}

	if (monsterBloodDrawTimeCalc > monsterBloodDrawTime)
	{
		monsterBloodInfo->GetMeshRenderer()->InactiveMeshRenderer();
		monsterBloodBorder->GetMeshRenderer()->InactiveMeshRenderer();
	}
	else
	{
		monsterBloodDrawTimeCalc += Time->GetDeltaTime();
		monsterBloodInfo->GetMeshRenderer()->GetMesh()->AdjustMeshWidthForRatio(monsterRemainBloodRatio);
	}
}