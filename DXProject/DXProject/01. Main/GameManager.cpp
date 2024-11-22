#include "pch.h"
#include "01. Main/GameManager.h"
#include "02. GameContents/ObjectPool/ObjectPool.h"
#include "00. Engine/Manager/RenderManager/RenderManager.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "00. Engine/Manager/TimeManager/TimeManager.h"

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