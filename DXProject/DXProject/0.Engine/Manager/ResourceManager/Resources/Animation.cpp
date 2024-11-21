#include "pch.h"
#include "Animation.h"

Animation::Animation() : Super(ResourceType::Animation)
{
	frames.reserve(8);
}

Animation::~Animation()
{

}

void Animation::SetTexture(Texture* _texture)
{
	texture = _texture;
}

void Animation::AddFrame(Frame frame)
{
	frames.push_back(frame);
}

bool Animation::GetLoop() const
{
	return loop;
}

void Animation::SetLoop(bool _loop)
{
	loop = _loop;
}

Frame Animation::GetFrameByIndex(int index)
{
	return frames[index];
}

const int Animation::GetFrameCount()
{
	return frames.size();
}

Texture* Animation::GetTexture()
{
	return texture;
}