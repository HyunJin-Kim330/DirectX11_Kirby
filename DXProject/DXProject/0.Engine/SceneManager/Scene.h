#pragma once

class GameObject;
class Collider;

class Scene
{
public:
	Scene(pair<float, float> cameraX, pair<float, float> cameraY, string _name);
	~Scene();

public:
	void Init();
	void Update();

	void AddGameObject(GameObject* gameObject);
	void RemoveGameObject(GameObject* gameObject, bool deleteObject = true);
	set<GameObject*> GetAllGameObject() { return gameObjectList; }
	vector<GameObject*> GetRenderableObject();
	vector<GameObject*> GetRenderableCollider();
	vector<Collider*> GetColliders(string tag);
	pair<float, float> GetCameraX() { return cameraX; }
	pair<float, float> GetCameraY() { return cameraY; }
	string GetName() const { return name; }

private:
	string name;
	set<GameObject*> gameObjectList;		// Scene�� �����ִ� gameObject��
	set<pair<GameObject*, bool>> removeObjectList;	// ��� gameObject�� ������Ʈ �� �����ؾ� �� ������Ʈ��
	const Vec3 kirbyInitPosition;
	const pair<float, float> cameraX;
	const pair<float, float> cameraY;

};
