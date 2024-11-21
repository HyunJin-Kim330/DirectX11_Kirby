#include "pch.h"
#include "GameManager.h"
#include "ObjectPool.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "TimeManager.h"

unique_ptr<GameManager> GGameManager = make_unique<GameManager>();

GameManager::GameManager()
{

}

GameManager::~GameManager()
{
	delete sceneManager;
	delete objectPool;
	delete resourceManager;
	delete renderManager;
	delete timeManager;
	delete inputManager;
}

void GameManager::Init(HWND _hwnd)
{
	hwnd = _hwnd;

	inputManager = new InputManager();
	inputManager->Init(hwnd);

	timeManager = new TimeManager();
	timeManager->Init();

	renderManager = new RenderManager(hwnd);
	renderManager->Init();

	resourceManager = new ResourceManager(renderManager->GetDevice());
	resourceManager->Init();

	objectPool = new ObjectPool();
	objectPool->Init();

	sceneManager = new SceneManager();
	sceneManager->Init();
	
}

void GameManager::Update()
{
	timeManager->Update();
	inputManager->Update();
	sceneManager->update();
	renderManager->Render();
}