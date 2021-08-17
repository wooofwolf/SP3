#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "SingletonPattern.h"

class Camera: public SingletonPattern<Camera>
{
	friend SingletonPattern<Camera>;
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	float camspeed = 100.f;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif