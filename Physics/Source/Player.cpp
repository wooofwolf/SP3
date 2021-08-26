#include "Player.h"
#include "Application.h"
#include "GL\glew.h"
#include "SceneBase.h"
//#include "SceneTest.h"
//#include "SceneLevel3.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{

}

void Player::Init(int m_worldWidth, int m_worldHeight, GunComponent* activeGun)
{
	this->m_worldWidth = m_worldWidth;
	this->m_worldHeight = m_worldHeight;

	hp = 100.f;
	pos.Set(1, 1, 2);
	prevpos.Set(1, 1, 2);
	scale.Set(3, 3, 3);
	normal.Set(1, 0, 0);
	vel.Set(0, 0, 0);
	active = true;

	m_speed = 1.f;
	SprintSpeed = 2.f;

	power1active = false;
	power2active = false;
	cooldownTimer2 = 0;

	this->activeGun = activeGun;

	energy = 100.f;
	dashCooldown = 0;
	dash = false;
	lose = false;
	nearX = false;
	nearY = false;
}

void Player::Update(double dt)
{
	prevpos = pos;
	
	if (hp <= 0)
	{
		SceneManager::GetInstance()->SetActiveScene("LoseScene");
		std::cout << "lost detected" << std::endl;
		lose = true;
	}
	// Energy Regeneration
	if (energy < 100.f)
	{
		if (power1active == false)
		{
			energy += 1.5 * dt;
		}
	}
	pos += vel * dt;
	vel *= 0.5;
	// Ability 1: Wide Area Slow
	static bool b1ButtonState = false;
	if (!b1ButtonState && Application::IsKeyPressed('1'))
	{
		b1ButtonState = true;

		power1active = !power1active;
	}
	else if (b1ButtonState && !Application::IsKeyPressed('1'))
		b1ButtonState = false;

	if (power1active == true)
	{
		SlowSpeed = 0.35f;
		
		if (energy > 0)
		{
			energy -= 7 * dt;
		}
		else
		{
			power1active = false;
		}
	}
	else
	{
		SlowSpeed = 1.f;
	}

	// Ability 2: Movement Speed/Firerate Increase
	static bool b2ButtonState = false;
	if (!b2ButtonState && Application::IsKeyPressed('2') && energy >= 50)
	{
		b2ButtonState = true;

		power2active = true;
		cooldownTimer2 = 0;

		energy -= 50;
	}
	else if (b2ButtonState && !Application::IsKeyPressed('2'))
		b2ButtonState = false;

	if (power2active == true)
	{
		cooldownTimer2 += dt;
		m_speed = 2.f;
		//std::cout << "activ" << std::endl;
	}

	if (cooldownTimer2 >= 5)
	{
		cooldownTimer2 = 0;
		//bQButtonState = false;
		power2active = false;
		m_speed = 1.f;
		//std::cout << "no activ" << std::endl;
	}

	// Ability 3: Time Grenade
	static bool bAbility3Pressed = false;
	if (!bAbility3Pressed && Application::IsKeyPressed('3') && energy >= 90)
	{
		Projectile* proj = new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_ABILITY_3);
		proj->Init();
		SceneManager::GetInstance()->GetActiveScene()->projectileList.push_back(proj);

		energy -= 90;
		bAbility3Pressed = true;
	}
	else if (bAbility3Pressed && !Application::IsKeyPressed('3'))
		bAbility3Pressed = false;

	// Movement
	if (Application::IsKeyPressed('W'))
	{
		pos.y += 30 * dt * m_speed;
		if (Application::IsKeyPressed(' ') && !dash && dashCooldown <= 0)
		{
			dash = true;
			vel.Set(0, 500, 0);
			if (dash )
			{
				dashTimer = 0;
			}
		}
	}	
	if (Application::IsKeyPressed('S'))
	{
		pos.y -= 30 * dt * m_speed;
		if (Application::IsKeyPressed(' ') && !dash && dashCooldown <= 0)
		{
			dash = true;
			vel.Set(0, -500, 0);
			if (dash)
			{
				dashTimer = 0;
			}
		}
	}	
	if (Application::IsKeyPressed('A'))
	{
		pos.x -= 30 * dt * m_speed;
		if (Application::IsKeyPressed(' ') && !dash && dashCooldown <= 0)
		{
			dash = true;
			vel.Set(-500, 0, 0);
			if (dash)
			{
				dashTimer = 0;
			}
		}
	}	
	if (Application::IsKeyPressed('D'))
	{
		pos.x += 30 * dt * m_speed;
		if (Application::IsKeyPressed(' ') && !dash && dashCooldown <= 0)
		{
			dash = true;
			vel.Set(500, 0, 0);
			if (dash)
			{
				dashTimer = 0;
			}
		}
	}
	if (dash)
	{
		dashTimer += dt;
		if (dashTimer >= 1)
		{
			dashCooldown = 1;
			dash = false;
		}
	}
	dashCooldown -= dt;
	//std::cout << dashCooldown << std::endl;

	int Xoffset = 88;
	int Yoffset = 55;

	if ((pos.x > 2 + 4 * 0 + Xoffset) && (pos.x < 4 * 200 - Xoffset))
	{
		nearX = false;

		Camera::GetInstance()->position.x = pos.x - m_worldWidth / 2;
		Camera::GetInstance()->target.x = pos.x - m_worldWidth / 2;
	}
	else
	{
		nearX = true;
	}
	if ((pos.y < m_worldHeight - Yoffset + 5) && (pos.y > m_worldHeight - 2 - 4 * 200 + Yoffset))
	{
		nearY = false;

		Camera::GetInstance()->position.y = pos.y - m_worldHeight / 2;
		Camera::GetInstance()->target.y = pos.y - m_worldHeight / 2;
	}
	else
	{
		nearY = true;
	}
}

void Player::Render()
{
}

void Player::Exit()
{
}

void Player::Movement(double dt)
{

}

Vector3 Player::GetPos()
{
	return pos;
}

Vector3 Player::GetRotate()
{
	return normal;
}

Vector3 Player::GetScale()
{
	return scale;
}

void Player::SetRotate(Vector3 rotate)
{
	this->normal = rotate;
}

void Player::SetPos(Vector3 pos)
{
	this->pos = pos;
}

float Player::GetPlusSpeed()
{
	return m_speed;
}

float Player::GetSlowSpeed()
{
	return SlowSpeed;
}

GunComponent* Player::GetActiveGun()
{
	return activeGun;
}

void Player::SetActiveGun(GunComponent* activeGun)
{
	this->activeGun = activeGun;
}

float Player::GetHealth()
{
	return hp;
}

void Player::SetHealth(float hp)
{
	this->hp = hp;
}

float Player::GetEnergy()
{
	return energy;
}

void Player::SetEnergy(int energy)
{
	this->energy = energy;
}

void Player::Collisioncheck(std::vector<Vector3> activeTileList)
{
	for (Vector3 tilepos : activeTileList)
	{
		if ((pos - tilepos).Length() < scale.x + 2)
		{
			pos = prevpos;
		}
		if ((pos - tilepos).Length() < scale.x + 5)
		{
			vel.SetZero();
		}
	}
}

void Player::Hazardcheck(std::vector<Vector3> activeTileList)
{
	for (Vector3 tilepos : activeTileList)
	{
		if ((pos - tilepos).Length() < scale.x + 2)
		{
			if (hp > 0.f)
			{
				hp -= 0.5f;
			}
		}
	}
}

void Player::Exitcheck(std::vector<Vector3> activeTileList)
{
	for (Vector3 tilepos : activeTileList)
	{
		if ((pos - tilepos).Length() < scale.x + 2)
		{
			switch (SceneManager::GetInstance()->GetActiveScene()->GetSceneType())
			{
			case (SceneBase::SCENE_TYPE::SCENE_LEVEL_1):
			{
				SceneManager::GetInstance()->SetActiveScene("Level2");
				break;
			}
			case (SceneBase::SCENE_TYPE::SCENE_LEVEL_2):
			{
				SceneManager::GetInstance()->SetActiveScene("Level3");
				break;
			}
			case (SceneBase::SCENE_TYPE::SCENE_LEVEL_3):
			{
				SceneManager::GetInstance()->SetActiveScene("Level4");
				break;
			}
			}
		}
	}
}

bool Player::nearXBoundary()
{
	return nearX;
}

bool Player::nearYBoundary()
{
	return nearY;
}