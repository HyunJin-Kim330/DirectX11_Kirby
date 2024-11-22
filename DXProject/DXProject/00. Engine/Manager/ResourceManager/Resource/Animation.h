#pragma once

class Texture;

struct Frame
{
	Vec2 offset = Vec2{ 0.f, 0.f };
	Vec2 size = Vec2{ 0.f, 0.f };
	float duration = 0.f;
};

class Animation : public ResourceBase
{
	using Super = ResourceBase;
public:
	Animation();
	~Animation();

	Texture* GetTexture();
	void SetTexture(Texture* texture);
	void SetLoop(bool _loop);

	bool GetLoop() const;
	Frame GetFrameByIndex(int index);
	const int GetFrameCount();

	void AddFrame(Frame frame);

private:
	Texture* texture = nullptr;
	vector<Frame> frames;
	bool loop = false;
	
};

