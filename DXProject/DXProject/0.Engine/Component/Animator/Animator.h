#pragma once

class Animation;

/*
	�ִϸ��̼� ���� ������Ʈ
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
	void InitFrameNum() { frameNum = 0; }			// �ִϸ��̼� ó������ ���
	void ActivateAnimator() { active = true; }		// �ִϸ��̼� ���
	void InactivateAnimator() { active = false; }		// �ִϸ��̼� ����
	void SetAnimationGater(string gatherName);

private:
	map<uint8, Animation*> animations;				// �ִϸ��̼� - ��ȣ�� ����
	Animation* presentAnimation;						// ���� �ִϸ��̼�
	float sumTime = 0.f;
	int frameNum = 0;
	bool active = true;

};

