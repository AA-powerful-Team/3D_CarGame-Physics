#include "PhysVehicle3D.h"

#include "Bullet/include/btBulletDynamicsCommon.h"
#include "p2Defs.h"
// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	Vehicle_parts.add(&chassis);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	
	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube chassis2(info.chassis2_size.x, info.chassis2_size.y, info.chassis2_size.z);
	Vehicle_parts.add(&chassis2);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis2.transform);
	btQuaternion p = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(info.chassis2_offset.x, info.chassis2_offset.y, info.chassis2_offset.z);
	offset2 = offset2.rotate(p.getAxis(), p.getAngle());
	
	chassis2.transform.M[12] += offset2.getX();
	chassis2.transform.M[13] += offset2.getY();
	chassis2.transform.M[14] += offset2.getZ();

	Cube chassis3(info.chassis3_size.x, info.chassis3_size.y, info.chassis3_size.z);
	Vehicle_parts.add(&chassis3);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis3.transform);
	btQuaternion r = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset3(info.chassis3_offset.x, info.chassis3_offset.y, info.chassis3_offset.z);
	offset3 = offset3.rotate(r.getAxis(), r.getAngle());

	chassis3.transform.M[12] += offset3.getX();
	chassis3.transform.M[13] += offset3.getY();
	chassis3.transform.M[14] += offset3.getZ();

	Cube chassis4(info.chassis4_size.x, info.chassis4_size.y, info.chassis4_size.z);
	Vehicle_parts.add(&chassis4);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis4.transform);
	btQuaternion x = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset4(info.chassis4_offset.x, info.chassis4_offset.y, info.chassis4_offset.z);
	offset4 = offset4.rotate(x.getAxis(), x.getAngle());

	chassis4.transform.M[12] += offset4.getX();
	chassis4.transform.M[13] += offset4.getY();
	chassis4.transform.M[14] += offset4.getZ();


	Cube chassis5(info.chassis5_size.x, info.chassis5_size.y, info.chassis5_size.z);
	Vehicle_parts.add(&chassis5);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis5.transform);
	btQuaternion z = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset5(info.chassis5_offset.x, info.chassis5_offset.y, info.chassis5_offset.z);
	offset5 = offset5.rotate(z.getAxis(), z.getAngle());

	chassis5.transform.M[12] += offset5.getX();
	chassis5.transform.M[13] += offset5.getY();
	chassis5.transform.M[14] += offset5.getZ();


	Cube chassis6(info.chassis6_size.x, info.chassis6_size.y, info.chassis6_size.z);
	Vehicle_parts.add(&chassis6);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis6.transform);
	btQuaternion k = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset6(info.chassis6_offset.x, info.chassis6_offset.y, info.chassis6_offset.z);
	offset6 = offset6.rotate(k.getAxis(), k.getAngle());

	chassis6.transform.M[12] += offset6.getX();
	chassis6.transform.M[13] += offset6.getY();
	chassis6.transform.M[14] += offset6.getZ();
	


	chassis6.Render();
	chassis5.Render();
	chassis4.Render();
	chassis3.Render();
	chassis2.Render();
	chassis.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			
			if(info.wheels[i].frontWheel==true)
			vehicle->setBrake(force*0.75, i);
			else
			vehicle->setBrake(force*0.25, i);


		}
	}
}

void PhysVehicle3D::Skid(float degrees) {

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].SkidWheel == true)
		{

		vehicle->setSteeringValue(degrees,i);
			
		}
	}


}
// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}
vec3 PhysVehicle3D::GetVehiclePos() {

	vec3 pos;


	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	/*btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());*/

	pos.x=chassis.transform.M[12];
	pos.y=chassis.transform.M[13];
	pos.z=chassis.transform.M[14];

	return pos;

}
vec3 PhysVehicle3D::GetDirectionVec() {
	
	vec3 ForwardVec;
	btVector3 vec = vehicle->getForwardVector();


	ForwardVec.x = vec.getX();
	ForwardVec.y = vec.getY();
	ForwardVec.z = vec.getZ();

	return ForwardVec;
}

bool PhysVehicle3D::overturned() {

	bool ret;
	vec3 AUX;
	btVector3 vec = vehicle->getUpVector();
	
	AUX.x = vec.getX();
	AUX.y = vec.getY();
	AUX.z = vec.getZ();



	if (AUX.y< 0) {
		ret = true;

	}
	else {

		ret = false;

	}


	return ret;
}


void PhysVehicle3D::StopVelocities()
{
	vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
	vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
}