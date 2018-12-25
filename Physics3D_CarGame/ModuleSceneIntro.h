#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "Timer.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

struct compact_info 
{
	Cube* cubeinfo;
	PhysBody3D* Pcubeinfo;
};


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CreatePath(int posx, int posy, int posz,int angle, vec3 rotation);
	void CreatePathWall(int posx, int posy, int posz, int angle, vec3 rotation,int type);
	void CreateRoad(int posx, int posy, int posz, int angle, vec3 rotation);
	void CreateCornerFloor(int posx, int posy, int posz);
	void CreateCorner(int posx, int posy, int posz, int side);
	void CreateRamp(int posx, int posy, int posz, int upordown);
	void createObstacle(int posx, int posy, int posz, int sizex, int sizey, int sizez,Color color, TypeObject type=TypeObject::NONE, bool is_sensor=false, bool wire=false);
	void CreateWorldBoundaries(); 
	void createBuilding(int posx, int posy, int posz, int sizex, int sizey, int sizez);
	void CreateSpeedBoost(int posx, int posy, int posz, int sizex, int sizey, int sizez, Color color);
	void CreateTurbine();

	PhysBody3D* CreateCubePhysbody(Cube* cube, Module* Callback, TypeObject type= TypeObject::NONE, bool is_sensor=false,float mass=0.00f);
	Cube* cubeCreation(vec3 position, vec3 size, Color rgb, float angle=0, vec3 pivot=0);

	compact_info SpinMachine(int posx, int posy, int posz, int sizex, int sizey, int sizez, float angle,float mass= 10000.0f);
	compact_info TorqueMove(int posx, int posy, int posz, float angle,float mass= 10000.0f);

	void createMap();
	void setObstacle();

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	// lis for clean up

	p2List<PhysBody3D*> cubePhysList;
	p2List<Cube*> cubeList;

	//sizes

	vec3 roadBorder = (100, 80, 1);
	vec3 roadSize = (200, 1, 1);


	//music
	uint StageMusic;
	uint LapSound;
	// timer and laps 
	int laps = 0;
	Timer Laptime;
	Timer Totaltime;
	Timer BlockT;
	Timer LapTimeSound;

	Uint32 Lap1, Lap2, Lap3;
	int lap1=0, lap2=0, lap3=0;
	int total_Time=0;
	bool restart = false;
	bool passLine = false;
	bool lock = false;

	//spining fans
	
	compact_info fan1;
	compact_info mover1;

	compact_info fan2;
	compact_info mover2;

	compact_info fan3;
	compact_info mover3;

	compact_info fan4;
	compact_info mover4;

	compact_info fan5;
	compact_info mover5;

	compact_info fan6;
	compact_info mover6;

	compact_info fan7;
	compact_info mover7;

	compact_info fan8;
	compact_info mover8;
};
