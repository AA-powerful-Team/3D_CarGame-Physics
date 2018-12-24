#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class Module;

enum class TypeObject {

	WALL_ROAD,
	BOOST_SPEED,
	VEHICLE,
	OUT_OF_BOUNDS,
	START,
	NONE


};
// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void AsSensor(bool is_sensor)const;


private:
	btRigidBody* body = nullptr;

public:

	bool mutable is_sensor;
	TypeObject type;
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__