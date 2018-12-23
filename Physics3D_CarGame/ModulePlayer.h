#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 100.0f
#define SKID_POWER 100.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:


	bool constantVelocity;

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

	Timer EngineSoundLoopTimer;
	Timer EngineConstantSpeedTimer;
	Timer EngineAcceleratingSoundTimer;
};