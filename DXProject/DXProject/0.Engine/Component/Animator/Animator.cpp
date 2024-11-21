#include "pch.h"
#include "Animator.h"
#include "Animation.h"

Animator::Animator() : Super(ComponentType::Animator)
{

}

Animator::~Animator()
{

}

void Animator::Update()
{
	if (animations.empty()) return;
	if (!active) return;

	float deltaTime = Time->GetDeltaTime();
	sumTime += deltaTime;

	Frame presentFrame = presentAnimation->GetFrameByIndex(frameNum);
	int frameCount = presentAnimation->GetFrameCount();

	if (sumTime >= presentFrame.duration)
	{
		if (frameNum == frameCount - 1)
		{
			if (presentAnimation->GetLoop())
			{
				frameNum = 0;
				sumTime = 0.f;
			}
		}
		else
		{
			frameNum++;
			sumTime = 0.f;
		}
	}
}

void Animator::SetPresentAnimation(uint8 _aniNum, bool maintainFrameNum)
{
	if (presentAnimation == animations[_aniNum]) return;
	presentAnimation = animations[_aniNum];
	
	if(!maintainFrameNum) frameNum = 0;
}

void Animator::ClearAnimations()
{
	animations.clear();
}

void Animator::AddAnimation(uint8 status, Animation* animation)
{
	animations[status] = animation;
}

void Animator::SetAnimationGater(string gatherName)
{
	map<int, Animation*> gatherAnimation = GAME->GetResourceManager()->GetAnimationGater(gatherName);
	map<int, Animation*>::iterator iter = gatherAnimation.begin();

	for (; iter != gatherAnimation.end(); ++iter)
	{
		AddAnimation(iter->first, iter->second);
	}
}