#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "Background.h"
#include "Boomer.h"
#include "Button.h"
#include "Camera.h"
#include "CarrotMonster.h"
#include "Collider.h"
#include "Door.h"
#include "DotCollider.h"
#include "EmptyObject.h"
#include "FollowPlayerAI.h"
#include "Foreground.h"
#include "GreenMonster.h"
#include "IceMonster.h"
#include "IdleAndAttackAI.h"
#include "IdleAndAttackWhenKirbyCloseAI.h"
#include "InformationUI.h"
#include "Kirby.h"
#include "KirbyCollisionBehavior.h"
#include "KingDedede.h"
#include "LineCollider.h"
#include "NormalAbility.h"
#include "NormalAI.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "RectCollider.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "SparkMonster.h"
#include "WaddleDee.h"
#include "WaddleDoo.h"
#include "Wall.h"
#include "WandererAI.h"

SceneManager::SceneManager()
{
	
}

SceneManager::~SceneManager()
{
	for (auto& scene : sceneList)
	{
		delete scene;
	}
}

struct Pred
{
	string findName;
	Pred(string _findName) : findName(_findName) {}
	
	bool operator()(const Scene* scene) const
	{
		return findName == scene->GetName();
	}
};

void SceneManager::Init()
{
	SetUbiquitousGameObject();
	LoadSceneFromFile(L"1-1");
	SetActiveScene(sceneList[0]);
}

void SceneManager::SetUbiquitousGameObject()
{
	{
		Kirby* kirby = new Kirby(new NormalAbility());
		kirby->GetTransform()->SetScale(Vec3{ 1.2f, 1.2f, 1.f });
		kirby->SetInitPosition(Vec3{ -4.f, 1.f, 0.f });
		kirby->GetTransform()->SetPosition(Vec3{ -4.f, 1.f, 0.f });
		{
			MeshRenderer* meshRenderer = new MeshRenderer();
			kirby->AddComponent(meshRenderer);
			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Bottom1x1");
			meshRenderer->SetMesh(mesh);
			auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
			meshRenderer->SetRenderingOrder(3);
		}
		{
			Animator* animator = new Animator();

			animator->SetAnimationGater("normal");
			animator->SetPresentAnimation(1);
			kirby->AddComponent(animator);
		}

		ubiquitousGameObject.push_back(kirby);
		this->kirby = kirby;

		{
			GameObject* temp = new EmptyObject();
			temp->SetInitPosition(Vec3{ -4.f, 1.f, 0.f });
			temp->SetParent(kirby);
			kirby->AddChild(temp);


			Collider* dotCollider = new DotCollider("KirbyDotCollider", 0.2f);
			dotCollider->AddCheckingTag("LineGrassGround");
			dotCollider->SetCollisionBehavior(new KirbyCollisionBehavior());
			vector<Vec3> vertices(1);
			vertices[0] = Vec3(0.f, 0.f, 0.f);
			dotCollider->SetVertices(vertices);
			temp->AddComponent(dotCollider);

			ubiquitousGameObject.push_back(temp);
		}
	}
	{
		// Camera
		GameObject* camera = new EmptyObject();
		this->camera = camera;
		camera->AddComponent(new Camera());
		ubiquitousGameObject.push_back(camera);
	}
	{
		//InformationUI
		InformationUI* informationUI = new InformationUI();
		this->informationUI = informationUI;
		ubiquitousGameObject.push_back(informationUI);
	}
}

void SceneManager::update()
{
	if (activeScene == nullptr) return;

	if (activeScene->GetName() == "wating")
	{
		if (sceneChangeTimeCalc > sceneChangeTime)
		{
			sceneChangeTimeCalc = 0.f;
			changeScene = false;

			vector<Scene*>::iterator iter = find_if(sceneList.begin(), sceneList.end(), Pred(beforeActiveScene));

			if (iter != sceneList.end())
			{
				SetActiveScene(*iter);
			}
		}
		else
		{
			sceneChangeTimeCalc += Time->GetDeltaTime();
		}
	}
	else
	{
		activeScene->Update();

		informationUI->UpdateInfo(kirby, monsterRemainBloodRatio);
		monsterRemainBloodRatio = -1.f;

		if (kirby->GetKirbyStatus() == KirbyStatus::Die)
		{
			changeScene = true;
			changeSceneName = "wating";
		}

		if (changeScene)
		{
			if (sceneChangeTimeCalc > sceneChangeTime)
			{
				vector<Scene*>::iterator iter = find_if(sceneList.begin(), sceneList.end(), Pred(changeSceneName));

				if (iter != sceneList.end())
				{
					SetActiveScene(*iter);
				}
				changeScene = false;
				sceneChangeTimeCalc = 0.f;
			}
			else
			{
				sceneChangeTimeCalc += Time->GetDeltaTime();
			}
		}
	}
}

void SceneManager::SetActiveScene(Scene* scene)
{
	if (activeScene != nullptr)
	{
		// 게임 오브젝트들 삭제
		beforeActiveScene = activeScene->GetName();
	}

	for (GameObject* gameObject : ubiquitousGameObject)
	{
		scene->AddGameObject(gameObject);
		for (GameObject* object : gameObject->GetChildren())
		{
			scene->AddGameObject(object);
		}
	}
	
	activeScene = scene;
	camera->GetCamera()->SetCameraMovePosition(scene->GetCameraX(), scene->GetCameraY());

	for (GameObject* gameObject : activeScene->GetAllGameObject())
	{
		if (gameObject->GetParent() != nullptr)
		{
			gameObject->GetParent()->Init();
		}
		gameObject->Init();
	}
	kirby->GetTransform()->SetPosition(Vec3{ -4.f, 1.f, 0.f });
	if (kirby->GetBlood() <= 0.f)
	{
		kirby->AlertChangeScene(true);
	}
	else
	{
		kirby->AlertChangeScene();

	}
	camera->GetTransform()->SetPosition(Vec3{ 0.f, 0.f, 0.f });
}

void SceneManager::ActivateScene(Scene* scene)
{

}

vector<GameObject*> SceneManager::GetRenderableObject()
{
	return activeScene->GetRenderableObject();
}

vector<GameObject*> SceneManager::GetRenderableCollider()
{
	return activeScene->GetRenderableCollider();
}

void SceneManager::LoadSceneFromFile(const wstring sceneInfo)
{
	sceneList.push_back(MakeScene1_1());
	sceneList.push_back(MakeScene1_3());
	sceneList.push_back(MakeScene1_5());
	sceneList.push_back(MakeWatingScene());
}

Scene* SceneManager::MakeScene1_1()
{
	Scene* scene = new Scene({0.f, 58.f}, {100.f, -100.f}, "1-1");

	{
		Background* background = new Background();
		background->SetInitPosition(Vec3{ -7.f, -0.3f, 0.f });
		background->SetFollowX({ 0.f, 58.f });
		background->SetFollowY({ 100.f, -100.f });
		MeshRenderer* meshRenderer = new MeshRenderer();
		background->AddComponent(meshRenderer);
		auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Left_26x10");
		meshRenderer->SetMesh(mesh);
		auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
		material->SetTexture(GAME->GetResourceManager()->Get<Texture>(L"background1"));
		meshRenderer->SetMaterial(material);
		meshRenderer->SetRenderingOrder(0);
		scene->AddGameObject(background);
	}
	{
		GameObject* foreground = new Foreground();
		foreground->SetInitPosition(Vec3{ 29.f, -4.69f, 0.f });
		{
			MeshRenderer* meshRenderer = new MeshRenderer();
			foreground->AddComponent(meshRenderer);
			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Bottom72.04x9.63");
			meshRenderer->SetMesh(mesh);
			auto material = GAME->GetResourceManager()->Get<Material>(L"1-1");
			meshRenderer->SetMaterial(material);
			meshRenderer->SetRenderingOrder(1);
		}
		scene->AddGameObject(foreground);

		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ -7.f, -1.8f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(35.f, 0.f, 0.f);
				vertices[2] = Vec3(35.f, -3.f, 0.f);
				vertices[3] = Vec3(0.f, -3.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 7.7f, -1.1f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(3.3f, 0.f, 0.f);
				vertices[2] = Vec3(3.3f, -1.f, 0.f);
				vertices[3] = Vec3(0.f, -1.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 19.25f, -1.15f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(0.6f, 0.f, 0.f);
				vertices[2] = Vec3(0.6f, -1.f, 0.f);
				vertices[3] = Vec3(0.f, -1.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 25.3f, -1.0f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(5.0f, 0.f, 0.f);
				vertices[2] = Vec3(5.0f, -3.f, 0.f);
				vertices[3] = Vec3(0.f, -3.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 29.6f, 1.05f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(2.6f, 0.f, 0.f);
				vertices[2] = Vec3(2.6f, -5.f, 0.f);
				vertices[3] = Vec3(0.f, -5.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 36.f, -1.1f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(12.6f, 0.f, 0.f);
				vertices[2] = Vec3(12.6f, -5.f, 0.f);
				vertices[3] = Vec3(0.f, -5.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 32.45f, 1.2f, 0.f });
			{
				Collider* collider = new LineCollider("LineGrassGround");
				vector<Vec3> vertices(2);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(4.5f, -2.4f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 47.9f, 2.5f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(1.8f, 0.f, 0.f);
				vertices[2] = Vec3(1.8f, -5.f, 0.f);
				vertices[3] = Vec3(0.f, -5.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 49.4f, 1.1f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(1.8f, 0.f, 0.f);
				vertices[2] = Vec3(1.8f, -5.f, 0.f);
				vertices[3] = Vec3(0.f, -5.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyOjbect = new EmptyObject();
			scene->AddGameObject(emptyOjbect);
			emptyOjbect->SetInitPosition(Vec3{ 51.f, -1.1f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(15.f, 0.f, 0.f);
				vertices[2] = Vec3(15.f, -3.f, 0.f);
				vertices[3] = Vec3(0.f, -3.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyOjbect->AddComponent(collider);
			}
		}
		{
			Door* emptyObject = new Door("1-3", Vec3{ -4.f, 0.f, 0.f }, Vec3{ 0.f, 0.f, 0.f });
			emptyObject->SetInitPosition(Vec3{ 63.3f, -1.1f, 0.f });
			scene->AddGameObject(emptyObject);
		}
		{
			EmptyObject* emptyObject = new EmptyObject();
			scene->AddGameObject(emptyObject);
			emptyObject->SetInitPosition(Vec3{ 2.8f, -1.95f, 0.f });
			MeshRenderer* meshRenderer = new MeshRenderer();
			emptyObject->AddComponent(meshRenderer);
			meshRenderer->SetRenderingOrder(1);

			Animator* animator = new Animator();
			emptyObject->AddComponent(animator);
			auto animation = GAME->GetResourceManager()->Get<Animation>(L"1-1Grass_1");
			animator->AddAnimation(0, animation);
			animator->SetPresentAnimation(0);

			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Bottom5.1x0.85");
			meshRenderer->SetMesh(mesh);
			auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
		}

		{
			EmptyObject* emptyObject = new EmptyObject();
			scene->AddGameObject(emptyObject);
			emptyObject->SetInitPosition(Vec3{ 31.f, 1.f, 0.f });
			MeshRenderer* meshRenderer = new MeshRenderer();
			emptyObject->AddComponent(meshRenderer);
			meshRenderer->SetRenderingOrder(1);

			Animator* animator = new Animator();
			emptyObject->AddComponent(animator);
			auto animation = GAME->GetResourceManager()->Get<Animation>(L"1-1Grass_2");
			animator->AddAnimation(0, animation);
			animator->SetPresentAnimation(0);

			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Bottom3.17x0.85");
			meshRenderer->SetMesh(mesh);
			auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
		}
		{
			EmptyObject* emptyObject = new EmptyObject();
			scene->AddGameObject(emptyObject);
			emptyObject->SetInitPosition(Vec3{ 54.1f, -1.15f, 0.f });
			MeshRenderer* meshRenderer = new MeshRenderer();
			emptyObject->AddComponent(meshRenderer);
			meshRenderer->SetRenderingOrder(1);

			Animator* animator = new Animator();
			emptyObject->AddComponent(animator);
			auto animation = GAME->GetResourceManager()->Get<Animation>(L"1-1Grass_3");
			animator->AddAnimation(0, animation);
			animator->SetPresentAnimation(0);

			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Bottom6.29x0.85");
			meshRenderer->SetMesh(mesh);
			auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
		}
	}
	{
		Monster* monster = new CarrotMonster();
		monster->InitMonster(Vec3{ 9.4f, -0.f, 0.f }, new WandererAI(true));
		scene->AddGameObject(monster);
	}
	{
		Monster* monster = new WaddleDee();
		monster->InitMonster(Vec3{ 22.f, -1.3f, 0.f }, new WandererAI(false));
		scene->AddGameObject(monster);
	}
	{
		Monster* monster = new WaddleDoo();
		monster->InitMonster(Vec3{ 45.f, -1.3f, 0.f }, new NormalAI());
		scene->AddGameObject(monster);
	}
	{
		Monster* monster = new SparkMonster();
		monster->InitMonster(Vec3{ 58.f, -1.3f, 0.f }, new FollowPlayerAI());
		scene->AddGameObject(monster);
	}

	return scene;
}

Scene* SceneManager::MakeScene1_3()
{
	Scene* scene = new Scene({0.f, 26.f}, {0.f, 10.2f}, "1-3");
	{
		Background* background = new Background();
		background->SetFollowX({ 0.f, 26.f });
		background->SetFollowY({ 0.f, 10.2f });
		background->SetInitPosition(Vec3{ -7.f, 2.4f, 0.f });
		MeshRenderer* meshRenderer = new MeshRenderer();
		background->AddComponent(meshRenderer);
		auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Left_26x10");
		meshRenderer->SetMesh(mesh);
		auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
		material->SetTexture(GAME->GetResourceManager()->Get<Texture>(L"background1"));
		meshRenderer->SetMaterial(material);
		meshRenderer->SetRenderingOrder(0);
		scene->AddGameObject(background);
	}
	{
		GameObject* foreground = new Foreground();
		foreground->SetInitPosition(Vec3{ -6.9f, 5.2f, 0.f });
		{
			MeshRenderer* meshRenderer = new MeshRenderer();
			foreground->AddComponent(meshRenderer);
			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Left74.0x37.0");
			meshRenderer->SetMesh(mesh);
			auto material = GAME->GetResourceManager()->Get<Material>(L"1-3");
			meshRenderer->SetMaterial(material);
			meshRenderer->SetRenderingOrder(1);
		}
		scene->AddGameObject(foreground);
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ -7.f, -2.3f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(35.f, 0.f, 0.f);
			vertices[2] = Vec3(35.f, -3.f, 0.f);
			vertices[3] = Vec3(0.f, -3.f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 20.1f, 6.5f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(3.4f, 0.f, 0.f);
			vertices[2] = Vec3(3.4f, -12.f, 0.f);
			vertices[3] = Vec3(0.f, -12.f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 4.4f, 3.f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(13.f, 0.f, 0.f);
			vertices[2] = Vec3(13.f, -1.7f, 0.f);
			vertices[3] = Vec3(0.f, -1.7f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 9.7f, 3.8f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(3.3f, 0.f, 0.f);
			vertices[2] = Vec3(3.3f, -1.7f, 0.f);
			vertices[3] = Vec3(0.f, -1.7f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ -6.8f, 4.7f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(11.2f, 0.f, 0.f);
			vertices[2] = Vec3(11.2f, -3.4f, 0.f);
			vertices[3] = Vec3(0.f, -3.4f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 3.6f, 1.3f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(1.6f, 0.f, 0.f);
			vertices[2] = Vec3(1.6f, -1.7f, 0.f);
			vertices[3] = Vec3(0.f, -1.7f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 27.9f, 14.5f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(10.6f, 0.f, 0.f);
			vertices[2] = Vec3(10.6f, -8.8f, 0.f);
			vertices[3] = Vec3(0.f, -8.8f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 17.5f, 3.f, 0.f });
		{
			Collider* collider = new RectCollider("ThinFloor");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(1.6f, 0.f, 0.f);
			vertices[2] = Vec3(1.6f, -0.2f, 0.f);
			vertices[3] = Vec3(0.f, -0.2f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 6.1f, 8.2f, 0.f });
		{
			Collider* collider = new RectCollider("ThinFloor");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(1.6f, 0.f, 0.f);
			vertices[2] = Vec3(1.6f, -0.2f, 0.f);
			vertices[3] = Vec3(0.f, -0.2f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 23.5f, 10.8f, 0.f });
		{
			Collider* collider = new RectCollider("ThinFloor");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(3.4f, 0.f, 0.f);
			vertices[2] = Vec3(3.4f, -0.2f, 0.f);
			vertices[3] = Vec3(0.f, -0.2f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 24.3f, 6.4f, 0.f });
		{
			Collider* collider = new RectCollider("ThinFloor");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(3.4f, 0.f, 0.f);
			vertices[2] = Vec3(3.4f, -0.2f, 0.f);
			vertices[3] = Vec3(0.f, -0.2f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 19.3f, 3.f, 0.f });
		{
			Collider* collider = new RectCollider("Ladder");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(0.6f, 0.f, 0.f);
			vertices[2] = Vec3(0.6f, -5.8f, 0.f);
			vertices[3] = Vec3(0.f, -5.8f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 5.4f, 8.2f, 0.f });
		{
			Collider* collider = new RectCollider("Ladder");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(0.6f, 0.f, 0.f);
			vertices[2] = Vec3(0.6f, -5.8f, 0.f);
			vertices[3] = Vec3(0.f, -5.8f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 15.f, 10.8f, 0.f });
		{
			Collider* collider = new RectCollider("Ladder");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(0.6f, 0.f, 0.f);
			vertices[2] = Vec3(0.6f, -4.f, 0.f);
			vertices[3] = Vec3(0.f, -4.f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 23.6f, 6.4f, 0.f });
		{
			Collider* collider = new RectCollider("Ladder");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(0.6f, 0.f, 0.f);
			vertices[2] = Vec3(0.6f, -3.6f, 0.f);
			vertices[3] = Vec3(0.f, -3.6f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 27.1f, 10.8f, 0.f });
		{
			Collider* collider = new RectCollider("Ladder");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(0.6f, 0.f, 0.f);
			vertices[2] = Vec3(0.6f, -4.2f, 0.f);
			vertices[3] = Vec3(0.f, -4.2f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 7.9f, 8.2f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(7.8f, 0.f, 0.f);
			vertices[2] = Vec3(7.8f, -1.6f, 0.f);
			vertices[3] = Vec3(0.f, -1.6f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 15.8f, 10.8f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(7.7f, 0.f, 0.f);
			vertices[2] = Vec3(7.7f, -4.2f, 0.f);
			vertices[3] = Vec3(0.f, -4.2f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 23.6f, 2.9f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(9.f, 0.f, 0.f);
			vertices[2] = Vec3(9.f, -4.2f, 0.f);
			vertices[3] = Vec3(0.f, -4.2f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 4.43f, 11.8f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(0.8f, 0.f, 0.f);
			vertices[2] = Vec3(0.8f, -8.8f, 0.f);
			vertices[3] = Vec3(0.f, -8.8f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		EmptyObject* emptyObject = new EmptyObject();
		scene->AddGameObject(emptyObject);
		emptyObject->SetInitPosition(Vec3{ 32.2f, 6.f, 0.f });
		{
			Collider* collider = new RectCollider("RectGrassGround");
			vector<Vec3> vertices(5);

			vertices[0] = Vec3(0.f, 0.f, 0.f);
			vertices[1] = Vec3(1.f, 0.f, 0.f);
			vertices[2] = Vec3(1.f, -4.f, 0.f);
			vertices[3] = Vec3(0.f, -4.f, 0.f);
			vertices[4] = Vec3(0.f, 0.f, 0.f);

			collider->SetVertices(vertices);
			emptyObject->AddComponent(collider);
		}
	}
	{
		Button* button = new Button(Vec3{ 0.2f, -2.3f, 0.f });
		button->SetInitPosition(Vec3{ 0.2f, -2.3f, 0.f });
		scene->AddGameObject(button->AddWall(Vec3{ 4.4f, -2.2f, 0.f }));
		scene->AddGameObject(button);

	}
	{
		Door* emptyObject = new Door("1-5", Vec3{ -4.f, 0.f, 0.f }, Vec3{ 0.f, 0.f, 0.f });
		emptyObject->SetInitPosition(Vec3{ 30.7f, 2.9f, 0.f });
		scene->AddGameObject(emptyObject);
	}
	{
		Monster* monster = new SparkMonster();
		monster->InitMonster(Vec3{ 11.6f, -1.f, 0.f }, new FollowPlayerAI());
		scene->AddGameObject(monster);
	}
	{
		Monster* monster = new Boomer();
		monster->InitMonster(Vec3{ 11.6f, 3.8f, 0.f }, new IdleAndAttackAI());
		scene->AddGameObject(monster);
	}
	{
		Monster* monster = new GreenMonster();
		monster->InitMonster(Vec3{ 11.6f, 8.2f, 0.f }, new WandererAI(false));

		scene->AddGameObject(monster);
	}
	{
		Monster* monster = new GreenMonster();
		monster->InitMonster(Vec3{ 27.7f, 2.9f, 0.f }, new WandererAI(false));
		scene->AddGameObject(monster);
	}
	{
		Monster* monster = new IceMonster();
		monster->InitMonster(Vec3{ 20.7f, 10.9f, 0.f }, new IdleAndAttackAI());
		scene->AddGameObject(monster);
	}
	{
		Monster* monster = new SparkMonster();
		monster->InitMonster(Vec3{ 25.5f, 9.5f, 0.f }, new IdleAndAttackWhenKirbyCloseAI(1.f));
		scene->AddGameObject(monster);
	}

	return scene;
}

Scene* SceneManager::MakeScene1_5()
{
	Scene* scene = new Scene({ 110.f, -100.f }, { 100.f, -100.f }, "1-5");
	{
		GameObject* foreground = new Foreground();
		foreground->SetInitPosition(Vec3{ -7.f, -0.3f, 0.f });
		scene->AddGameObject(foreground);

		{
			MeshRenderer* meshRenderer = new MeshRenderer();
			foreground->AddComponent(meshRenderer);
			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Left14.0x10.7");
			meshRenderer->SetMesh(mesh);
			auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
			meshRenderer->SetRenderingOrder(1);

			Animator* animator = new Animator();
			foreground->AddComponent(animator);
			auto animation = GAME->GetResourceManager()->Get<Animation>(L"KingDededeArea");
			animator->AddAnimation(0, animation);
			animator->SetPresentAnimation(0);
		}
		{
			EmptyObject* emptyObject = new EmptyObject();
			scene->AddGameObject(emptyObject);
			emptyObject->SetInitPosition(Vec3{ -7.6f, -3.f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(17.6f, 0.f, 0.f);
				vertices[2] = Vec3(17.6f, -7.7f, 0.f);
				vertices[3] = Vec3(0.f, -7.7f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyObject->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyObject = new EmptyObject();
			scene->AddGameObject(emptyObject);
			emptyObject->SetInitPosition(Vec3{ -9.f, 5.f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(2.f, 0.f, 0.f);
				vertices[2] = Vec3(2.f, -8.f, 0.f);
				vertices[3] = Vec3(0.f, -8.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyObject->AddComponent(collider);
			}
		}
		{
			EmptyObject* emptyObject = new EmptyObject();
			scene->AddGameObject(emptyObject);
			emptyObject->SetInitPosition(Vec3{ 7.f, 6.f, 0.f });
			{
				Collider* collider = new RectCollider("RectGrassGround");
				vector<Vec3> vertices(5);

				vertices[0] = Vec3(0.f, 0.f, 0.f);
				vertices[1] = Vec3(2.f, 0.f, 0.f);
				vertices[2] = Vec3(2.f, -9.f, 0.f);
				vertices[3] = Vec3(0.f, -9.f, 0.f);
				vertices[4] = Vec3(0.f, 0.f, 0.f);

				collider->SetVertices(vertices);
				emptyObject->AddComponent(collider);
			}
		}
		{
			Monster* monster = new KingDedede();
			scene->AddGameObject(monster);
			monster->SetInitPosition(Vec3{ 4.5f, -3.f, 0.f });
		}
	}
	return scene;
}

Scene* SceneManager::MakeWatingScene()
{
	Scene* scene = new Scene({ 0.f, 58.f }, { 100.f, -100.f }, "wating");
	EmptyObject* emptyOjbect = new EmptyObject();
	emptyOjbect->GetTransform()->SetPosition(Vec3{ 0.f, 0.f, 0.f });
	scene->AddGameObject(emptyOjbect);
	return scene;
}

Vec3 SceneManager::GetKirbyPosition()
{
	if (kirby == nullptr)
	{
		return Vec3{ 0.f, 0.f, 0.f };
	}

	return kirby->GetTransform()->GetPosition();
}

Vec3 SceneManager::GetKirbyVelocity()
{
	if (kirby == nullptr)
	{
		return Vec3{ 0.f, 0.f, 0.f };
	}
	return kirby->GetVelocity();
}

Vec3 SceneManager::GetCameraPosition()
{
	if (camera == nullptr)
	{
		return Vec3{ 0.f, 0.f, 0.f };
	}
	return camera->GetTransform()->GetPosition();
}