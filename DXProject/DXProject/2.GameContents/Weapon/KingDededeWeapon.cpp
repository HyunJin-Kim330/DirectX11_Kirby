#include "pch.h"
#include "AttackBasic.h"
#include "KingDededeWeapon.h"
#include "LineCollider.h"

KingDededeWeapon::KingDededeWeapon()
{
	damage = 20.f;

	Collider* weaponCollider = new LineCollider("BeamWeapon");
	weaponCollider->AddCheckingTag("KirbyBasic");
	weaponCollider->SetCollisionBehavior(new AttackBasic());
	AddComponent(weaponCollider);
}

KingDededeWeapon::~KingDededeWeapon()
{

}

void KingDededeWeapon::Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner)
{
	attackDirection = direction;

	if (GetParent() == nullptr)
	{
		summoner->AddChild(this);
		SetParent(summoner);
	}

	if (GetCollider() != nullptr)
	{
		GetCollider()->Activate();
	}

	if (attackDirection == Direction::RIGHT)
	{
		angle = 130;
		GetTransform()->SetPosition(summonerPosition + Vec3{ 0.f, 0.6f, 0.f });
	}
	else
	{
		angle = 50;
		GetTransform()->SetPosition(summonerPosition + Vec3{ -0.f, 0.6f, 0.f });
	}

	vector<Vec3> vertices(2);
	float x = cosf(angle * (3.141592 / 180)) * 3.5f;
	float y = sinf(angle * (3.141592 / 180)) * 3.5f;
	vertices[0] = Vec3(0.f, 0.f, 0.f);
	vertices[1] = Vec3(x, y, 0.f);
	GetCollider()->SetVertices(vertices);
}

void KingDededeWeapon::UpdateObject()
{
	attackTime += Time->GetDeltaTime();

	Collider* weaponCollider = GetCollider();
	vector<Vec3> vertices = weaponCollider->GetOriginalVertices();

	if (attackDirection == Direction::RIGHT)
	{
		if (user == UserType::Kirby) angle -= 5.f;
		else angle -= 1.f;
	}
	else
	{
		if (user == UserType::Kirby) angle += 5.f;
		else angle += 1.f;
	}

	float x = cosf(angle * (3.141592 / 180)) * 3.5f;
	float y = sinf(angle * (3.141592 / 180)) * 3.5f;
	vertices[1] = Vec3(x, y, 0.f);
	weaponCollider->SetVertices(vertices);

	if (user == UserType::Kirby && attackTime >= 0.5f)
	{
		StopAttack();
	}

	if (user == UserType::Monster && attackTime >= 0.9f)
	{
		StopAttack();
	}
}