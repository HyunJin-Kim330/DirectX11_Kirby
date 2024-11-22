#pragma once

class Animation;

/*
	애니메이션 관련 컴포넌트
*/
class Animator : public Component
{
	using Super = Component;

public:
	Animator();
	~Animator();
	
public :
	virtual void Update() override;

	Animation* GetPresentAnimation() const { return presentAnimation; }
	int GetFrameNum() const { return frameNum; }

	void SetPresentAnimation(uint8 _aniNum, bool maintainFrameNum = false);
	
	void ClearAnimations();
	void AddAnimation(uint8 status, Animation* animation);
	void InitFrameNum() { frameNum = 0; }			// 애니메이션 처음부터 재생
	void ActivateAnimator() { active = true; }		// 애니메이션 재생
	void InactivateAnimator() { active = false; }		// 애니메이션 멈춤
	void SetAnimationGater(string gatherName);

private:
	map<uint8, Animation*> animations;				// 애니메이션 - 번호로 저장
	Animation* presentAnimation;						// 현재 애니메이션
	float sumTime = 0.f;
	int frameNum = 0;
	bool active = true;

};

