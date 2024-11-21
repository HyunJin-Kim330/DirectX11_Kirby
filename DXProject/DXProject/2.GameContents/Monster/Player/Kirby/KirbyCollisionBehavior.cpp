#include "pch.h"
#include "Ability.h"
#include "Button.h"
#include "Door.h"
#include "Kirby.h"
#include "KirbyCollisionBehavior.h"
#include "Monster.h"
#include "LineCollider.h"

void KirbyCollisionBehavior::Behavior(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	string otherColliderTag = other->GetTag();
	string standardTag = standard->GetTag();
	Kirby* kirby = nullptr;

	if (standardTag == "KirbyBasic")
	{
		kirby = dynamic_cast<Kirby*>(standard->GetGameObject());
	}
	else if (standardTag == "KirbyDotCollider")
	{
		kirby = dynamic_cast<Kirby*>(standard->GetGameObject()->GetParent());
	}
	else
	{
		return;
	}

	if (otherColliderTag == "RectGrassGround")
	{
		if (standardTag == "KirbyBasic")
		{
			if (colDirection == CollisionDirection::Bottom)
			{
				kirby->SetGrounded(true);
				kirby->SetCollideWithBottomGrass(true);

				float distance = other->GetVertices()[0].y - standard->GetVertices()[2].y;
				if (distance > 0.f)
				{
					Vec3 presentPosition = kirby->GetTransform()->GetPosition();
					kirby->GetTransform()->SetPosition(Vec3{ presentPosition.x, presentPosition.y + distance, 0.f });
				}
			}
			else if (colDirection == CollisionDirection::Left || colDirection == CollisionDirection::Right)
			{
				kirby->AlertBlockSide(colDirection);
			}
			else if (colDirection == CollisionDirection::Top)
			{
				kirby->AlertBlockTop();
			}
		}
	}
	else if (otherColliderTag == "ThinFloor")
	{
		vector<Vec3> standardVertices = standard->GetVertices();
		vector<Vec3> otherVertices = other->GetVertices();

		if (standardVertices[3].y > otherVertices[0].y)
		{
			if (kirby->GetKirbyStatus() != KirbyStatus::FloatingUp)
			{
				if (colDirection == CollisionDirection::Bottom)
				{
					kirby->SetGrounded(true);
					kirby->SetCollideWithBottomGrass(true);

					float distance = other->GetVertices()[0].y - standard->GetVertices()[2].y;
					if (distance > 0.f)
					{
						Vec3 presentPosition = kirby->GetTransform()->GetPosition();
						kirby->GetTransform()->SetPosition(Vec3{ presentPosition.x, presentPosition.y + distance, 0.f });
					}
				}
			}
		}
	}
	else if (otherColliderTag == "LineGrassGround")
	{
		if (standardTag == "KirbyDotCollider")
		{
			LineCollider* lineCollider = dynamic_cast<LineCollider*>(other);

			kirby->SetGrounded(true);
			if (lineCollider->GetTiltLeft())
			{
				kirby->SetCollideWithLeftRampGrass(true);
			}
			else
			{
				kirby->SetCollideWithRightRampGrass(true);

			}
			kirby->SetTiltLeftDirection(lineCollider->GetLeftDirectionVec());
			kirby->SetTiltRightDirection(lineCollider->GetRightDirectionVec());
		}
	}
	else if (otherColliderTag == "Door")
	{
		if (standardTag == "KirbyBasic")
		{
			kirby->SetFrontDoor(true);
			
			if (GAME->GetInputManager()->GetButtonDown(KEY_TYPE::UP))
			{
				Door* door = dynamic_cast<Door*>(other->GetGameObject());
				GAME->GetSceneManager()->NeedChangeScene(door->GetSceneName());
			}
		}
	}
	else if (otherColliderTag == "Ladder")
	{
		vector<Vec3> standardVertices = standard->GetVertices();
		vector<Vec3> otherVertices = other->GetVertices();
		
		if (standardVertices[3].y > otherVertices[0].y - 0.2f)
		{
			kirby->SetOnTheLedderTop(true);

			if (kirby->GetKirbyStatus() == KirbyStatus::Jumping || kirby->GetKirbyStatus() == KirbyStatus::UpLadder) return;

			kirby->SetGrounded(true);
			kirby->SetCollideWithBottomGrass(true);

			if (Input->GetButton(KEY_TYPE::DOWN)) 
				return;
			float distance = other->GetVertices()[0].y - standard->GetVertices()[2].y;
			if (distance > 0.f)
			{
				Vec3 presentPosition = kirby->GetTransform()->GetPosition();
				kirby->GetTransform()->SetPosition(Vec3{ presentPosition.x, presentPosition.y + distance, 0.f });
			}
		}
		else
		{
			if (standardVertices[3].y < otherVertices[0].y - 0.5f)
			{
				kirby->SetOnTheLedderTop(false);
				kirby->SetFrontLadder(true);
			}
		}
	}
	else if (otherColliderTag == "Button")
	{
		Button* button = dynamic_cast<Button*>(other->GetGameObject());
		button->PushButton();
	}
}

void KirbyCollisionBehavior::NotAtAllCollideBehavior(set<pair<string, CollisionDirection>> collidedTag, Collider* c)
{
	set<pair<string, CollisionDirection>>::iterator iter = collidedTag.find({ "RectGrassGround", CollisionDirection::Bottom });
	set<pair<string, CollisionDirection>>::iterator iter2 = collidedTag.find({ "ThinFloor", CollisionDirection::Bottom });
	set<pair<string, CollisionDirection>>::iterator iter3 = collidedTag.find({ "LadderTop", CollisionDirection::Bottom });
	set<pair<string, CollisionDirection>>::iterator iter4 = collidedTag.find({ "LadderTop", CollisionDirection::Top });

	Kirby* kirby = nullptr;

	if (c->GetTag() == "KirbyBasic")
	{
		kirby = dynamic_cast<Kirby*>(c->GetGameObject());

		iter = collidedTag.find({ "Ladder", CollisionDirection::Left });
		iter2 = collidedTag.find({ "Ladder", CollisionDirection::Right });
		iter3 = collidedTag.find({ "Ladder", CollisionDirection::Top });
		iter4 = collidedTag.find({ "Ladder", CollisionDirection::Bottom });

		if (iter == collidedTag.end() && iter2 == collidedTag.end() && iter3 == collidedTag.end() && iter4 == collidedTag.end())
		{
			kirby->SetOnTheLedderTop(false);
		}
		iter = collidedTag.find({ "RectGrassGround", CollisionDirection::Bottom });
		iter2 = collidedTag.find({ "ThinFloor", CollisionDirection::Bottom });
		if (iter == collidedTag.end() && iter2 == collidedTag.end() && !kirby->GetOnTheLadderTop())
		{
			//kirby->NotCollideAtAllWithGroundBottom();
			kirby->SetCollideWithBottomGrass(false);
		}

		iter = collidedTag.find({ "RectGrassGround", CollisionDirection::Left });
		iter2 = collidedTag.find({ "RectGrassGround", CollisionDirection::Right });

		if (iter == collidedTag.end() && iter2 == collidedTag.end())
		{
			kirby->NotCollideAtAllWithGroundSide();
		}

		iter = collidedTag.find({ "Door", CollisionDirection::Left });
		iter2 = collidedTag.find({ "Door", CollisionDirection::Right });
		iter3 = collidedTag.find({ "Door", CollisionDirection::Top });
		iter4 = collidedTag.find({ "Door", CollisionDirection::Bottom });

		if (iter == collidedTag.end() && iter2 == collidedTag.end() && iter3 == collidedTag.end() && iter4 == collidedTag.end())
		{
			kirby->SetFrontDoor(false);
		}

		iter = collidedTag.find({ "Ladder", CollisionDirection::Left });
		iter2 = collidedTag.find({ "Ladder", CollisionDirection::Right });
		iter3 = collidedTag.find({ "Ladder", CollisionDirection::Top });
		iter4 = collidedTag.find({ "Ladder", CollisionDirection::Bottom });

		if (iter == collidedTag.end() && iter2 == collidedTag.end() && iter3 == collidedTag.end() && iter4 == collidedTag.end())
		{
			kirby->SetFrontLadder(false);
		}
	}
	else if (c->GetTag() == "KirbyDotCollider")
	{
		kirby = dynamic_cast<Kirby*>(c->GetGameObject()->GetParent());
		iter = collidedTag.find({ "LineGrassGround", CollisionDirection::Left });
		if (iter == collidedTag.end())
		{
			kirby->SetCollideWithLeftRampGrass(false);
			kirby->SetCollideWithRightRampGrass(false);
		}
	}
}