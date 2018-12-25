#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1500.0f
#define TURN_DEGREES 8.5f * DEGTORAD
#define BRAKE_POWER 100.0f
#define SKID_POWER 100.0f
#define BOOST_POWER 1500.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void MusicConditions();
	void resetPlayerPos();
	void resetOrientationPos();
	void addVehicle();

public:


	bool constantVelocity;
	bool Boost = false;
	bool Menu = true;
	bool playMusic = true;

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	float skid;
	float CameraZoom=12.00;
	float cameraAcceleration = 0.02;
	float CurrentVelocity;



	uint RevEngineSound;
	uint StartingEngine;
	uint EngineLoop;
	uint constantSpeedEngine;
	uint AcceleratingEngine;
	uint BrakingSound;
	uint Turning;
	uint TireSkid;
	uint BoostSound;

	Timer EngineSoundLoopTimer;
	Timer EngineConstantSpeedTimer;
	Timer EngineAcceleratingSoundTimer;
	Timer BoostTime;

	mat4x4 matrix;
};