#include "Player.h"
#include "Application.h"
#include "GL\glew.h"
#include "SceneBase.h"
#include "SceneTest.h"

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

	hp = 100;
	pos.Set(1, 1, 1);
	prevpos.Set(1, 1, 1);
	scale.Set(3, 3, 3);
	normal.Set(1, 0, 0);
	active = true;

	m_speed = 1.f;
	SprintSpeed = 2.f;

	power1active = false;
	power2active = false;
	cooldownTimer2 = 0;
	cooldownTimer3 = 30;

	this->activeGun = activeGun;

	energy = 100.f;
}

void Player::Update(double dt)
{
	prevpos = pos;
	
	// Energy Regeneration
	if (energy < 100.f)
	{
		if (power1active == false)
		{
			energy += 1.5 * dt;
		}
	}

	// Ability 1
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
		SlowSpeed = 0.5f;
		
		if (energy > 0)
		{
			energy -= 2 * dt;
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

	// Ability 2
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
	cooldownTimer3 += dt;

	static bool bAbility3Pressed = false;
	if (!bAbility3Pressed && Application::IsKeyPressed('3') && cooldownTimer3 >= 30.f && energy >= 90)
	{
		Projectile* proj = new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_ABILITY_3);
		proj->Init();
		SceneTest::GetInstance()->projectileList.push_back(proj);

		energy -= 90; 
		cooldownTimer3 = 0;
		bAbility3Pressed = true;
	}
	else if (bAbility3Pressed && !Application::IsKeyPressed('3'))
		bAbility3Pressed = false;

	// Movement
	if (Application::IsKeyPressed('W'))
	{
		pos.y += 30 * dt * m_speed;
	}	
	if (Application::IsKeyPressed('S'))
	{
		pos.y -= 30 * dt * m_speed;
	}	
	if (Application::IsKeyPressed('A'))
	{
		pos.x -= 30 * dt * m_speed;
	}	
	if (Application::IsKeyPressed('D'))
	{
		pos.x += 30 * dt * m_speed;
	}
}

void Player::Render()
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelStack.LoadIdentity();

	modelStack.PushMatrix();
	{
		modelStack.Translate(pos.x, pos.y, 0);
		modelStack.Rotate(Math::RadianToDegree(atan2f(m_worldHeight - (y / h) * m_worldHeight - pos.y, (x / w) * m_worldWidth - pos.x)), 0, 0, 1);
		modelStack.Scale(scale.x, scale.y * 0.5, scale.z);
		//RenderMesh(meshlist->GetMesh(MeshList::GEO_CUBE_RED), false);
	}
	modelStack.PopMatrix();
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

int Player::GetHealth()
{
	return hp;
}

void Player::SetHealth(int hp)
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
	}
}