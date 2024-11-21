#pragma once

class SceneManager;
class RenderManager;
class ObjectPool;

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Init(HWND _hwnd);
	void Update();

	InputManager* GetInputManager() { return inputManager; }
	TimeManager* GetTimeManager() { return timeManager; }
	SceneManager* GetSceneManager() { return sceneManager; }
	ResourceManager* GetResourceManager() { return resourceManager; }
	RenderManager* GetRenderManager() { return renderManager; }
	ObjectPool* GetObjectPool() { return objectPool; }

private:
	HWND hwnd;
	InputManager* inputManager;
	TimeManager* timeManager;
	SceneManager* sceneManager;
	ResourceManager* resourceManager;
	RenderManager* renderManager;
	ObjectPool* objectPool;

private:


};

extern unique_ptr<GameManager> GGameManager;

