#include "pch.h"
#include "Background.h"
#include "SceneManager.h"

Background::Background()
{

}

Background::~Background()
{

}

void Background::UpdateObject()
{
	Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
	Vec3 kirbyVelocity = GAME->GetSceneManager()->GetKirbyVelocity();

	float x = 0.f;
	float y = 0.f;
	Vec3 presentPosition = GetTransform()->GetPosition();

	if (followX.first < kirbyPosition.x && kirbyPosition.x < followX.second)
	{
		if (Input->GetButton(KEY_TYPE::RIGHT))
		{
			x = fabsf(kirbyVelocity.x - 0.35f);
			//GetTransform()->SetPosition(presentPosition + Vec3{ fabsf(kirbyVelocity.x - 0.35f), 0.f, 0.f} * Time->GetDeltaTime());
		}
		else if (Input->GetButton(KEY_TYPE::LEFT))
		{
			x = -fabsf(kirbyVelocity.x + 0.35f);

			//GetTransform()->SetPosition(presentPosition + Vec3{ -fabsf(kirbyVelocity.x + 0.35f), 0.f, 0.f } * Time->GetDeltaTime());
		}
		
	}

	if (followY.first < kirbyPosition.y && kirbyPosition.y < followY.second)
	{
		if (Input->GetButton(KEY_TYPE::UP))
		{
			//y = -fabsf(kirbyVelocity.y + 0.35f);

			y = fabsf(kirbyVelocity.y / 2.f);
			//GetTransform()->SetPosition(presentPosition + Vec3{ fabsf(kirbyVelocity.x - 0.35f), 0.f, 0.f} * Time->GetDeltaTime());
		}
		else if (Input->GetButton(KEY_TYPE::DOWN))
		{
			//y = fabsf(kirbyVelocity.y - 0.35f);

			y = -fabsf(kirbyVelocity.y / 2.f);

			//GetTransform()->SetPosition(presentPosition + Vec3{ -fabsf(kirbyVelocity.x + 0.35f), 0.f, 0.f } * Time->GetDeltaTime());
		}

	}

	if (kirbyVelocity.x == 0.f) x = 0.f;
	if (kirbyVelocity.y == 0.f) y = 0.f;

	GetTransform()->SetPosition(presentPosition + Vec3{ x, y, 0.f } * Time->GetDeltaTime());

}