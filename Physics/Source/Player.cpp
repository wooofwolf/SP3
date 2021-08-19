#include "Player.h"
#include "Application.h"
#include "GL\glew.h"
#include "SceneBase.h"

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

	power2active = false;

	this->activeGun = activeGun;
}

void Player::Update(double dt)
{
	prevpos = pos;
	// Currently Hold Space to slow time, Change to a timer toggle later
	if (Application::IsKeyPressed('1'))
	{
		SlowSpeed = 0.5f;
	}
	else
	{
		SlowSpeed = 1.f;
	}	

	static bool bQButtonState = false;
	if (!bQButtonState && Application::IsKeyPressed('2'))
	{
		bQButtonState = true;
		if (power2active == false)
		{
			m_speed = 2.f;
			power2active = true;
			std::cout << "activ" << std::endl;
		}
		else if (power2active == true)
		{
			m_speed = 1.f;
			power2active = false;
			std::cout << "no activ" << std::endl;
		}

	}
	else if (bQButtonState && !Application::IsKeyPressed('2'))
		bQButtonState = false;

	

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