#pragma once
#include "Scene.h"
class InformationUI;
class Kirby;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

public:
	Scene* GetActiveScene() { return activeScene; }
	vector<GameObject*> GetRenderableObject();
	vector<GameObject*> GetRenderableCollider();
	Vec3 GetKirbyPosition();
	Vec3 GetKirbyVelocity();
	Vec3 GetCameraPosition();

	void SetActiveScene(Scene* scene);
	void SetMonsterRemainBloodRatio(float val) { monsterRemainBloodRatio = val; }
	void NeedChangeScene(string _changeSceneName) { changeScene = true; changeSceneName = _changeSceneName; }

	void Init();
	void update();  // active scene을 업데이트

private:
	vector<GameObject*> ubiquitousGameObject;
	vector<Scene*> sceneList;
	Scene* activeScene = nullptr;
	string beforeActiveScene = "";
	Kirby* kirby = nullptr;
	GameObject* camera = nullptr;
	InformationUI* informationUI = nullptr;
	float monsterRemainBloodRatio = -1.f;
	float sceneChangeTime = 0.6f;
	float sceneChangeTimeCalc = 0.f;
	bool changeScene = false;
	string changeSceneName = "";

	Scene* MakeScene1_1();
	Scene* MakeScene1_3();
	Scene* MakeScene1_5();
	Scene* MakeWatingScene();

private:
	void LoadSceneFromFile(const wstring sceneInfo);
	void ActivateScene(Scene* scene);
	void SetUbiquitousGameObject();
};

