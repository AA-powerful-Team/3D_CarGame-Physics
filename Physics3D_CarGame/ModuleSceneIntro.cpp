#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateRoad(38, 0, -190, 90, vec3(0, 1, 0));
	CreateRoad(38, 0, -90, 90, vec3(0, 1, 0));
	CreateRoad(38, 0, 10, 90, vec3(0, 1, 0));

	CreateRoad(100, 0, 70, 0, vec3(1, 0, 0));
	CreateRoad(200, 0, 70, 0, vec3(1, 0, 0));
	CreateRoad(300, 0, 70, 0,vec3(0,1,0));

	CreateRoad(360, 0, 10, 90, vec3(0, 1, 0));
	CreateRoad(360, 0, -90, 90, vec3(0, 1, 0));
	CreateRoad(360, 0, -190, 90, vec3(0, 1, 0));

	CreateRoad(100, 0, -250, 0, vec3(1, 0, 0));
	CreateRoad(200, 0, -250, 0, vec3(1, 0, 0));
	CreateRoad(300, 0, -250, 0, vec3(0, 1, 0));

	//CreateCorner(0,0,0,1);

	//CreateCorner(0, 0, +30, 2);

	//CreateCorner(-30, 0, 0, 4);

	//CreateCorner(-30, 0, +30, 3);

	CreateRamp(0, 0, 60, 1);

	CreateRamp(0, 0, 90, 2);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	
	p2List_item <Cube*>* cube_render = cubeList.getFirst();
	while (cube_render != nullptr) {
		
		cube_render->data->Render();
		cube_render = cube_render->next;
	
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreatePath(int posx, int posy, int posz, int angle,vec3 rotation) {

	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(100,1,20), Grey, angle, rotation);
	cubePhysList.add(CreateCubePhysbody(cube, this));
	cubeList.add(cube);
}

void ModuleSceneIntro::CreatePathWall(int posx, int posy, int posz, int angle, vec3 rotation, int type)
{
	if (type == 1)
	{
		Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(100, 3, 1), DarkGrey, angle, rotation);
		cubePhysList.add(CreateCubePhysbody(cube, this));
		cubeList.add(cube);
	}
	else if (type == 2)
	{
		Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(30, 3, 1), DarkGrey, angle, rotation);
		cubePhysList.add(CreateCubePhysbody(cube, this));
		cubeList.add(cube);
	}
	
	
}

void ModuleSceneIntro::CreateRoad(int posx, int posy, int posz, int angle,vec3 rotation )
{
	CreatePath(posx, posy, posz, angle, rotation);
	if (angle != 0)
	{
		CreatePathWall(posx+10, posy + 2, posz, angle, vec3(rotation.x, rotation.y, rotation.z),1);
		CreatePathWall(posx-10, posy, posz, angle, rotation,1);
	}
	else
	{
		CreatePathWall(posx, posy + 2, posz + 10, angle, vec3(rotation.x, rotation.y, rotation.z),1);
		CreatePathWall(posx, posy, posz - 10, angle, rotation,1);
	}
	
}

Cube* ModuleSceneIntro::cubeCreation(vec3 position, vec3 size, Color rgb, float angle, vec3 pivot) {
	Cube* cube = new Cube;
	cube->SetRotation(angle, pivot);
	cube->SetPos(position.x, position.y, position.z);
	cube->size = size;
	cube->wire = false;
	cube->color = rgb;
	
	return cube;
}

PhysBody3D* ModuleSceneIntro::CreateCubePhysbody(Cube* cube, Module* Callback) {

	PhysBody3D* cubeP;
	cubeP = App->physics->AddBody(*cube, 0.0f);
	cubeP->collision_listeners.add(Callback);

	return cubeP;
}

void ModuleSceneIntro::CreateCornerFloor(int posx, int posy, int posz)
{
	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(30, 1, 30), Grey, 0, vec3(1,0,0));
	cubePhysList.add(CreateCubePhysbody(cube, this));
	cubeList.add(cube);
}


void ModuleSceneIntro::CreateCorner(int posx, int posy, int posz, int side)
{

	if (side == 1)
	{
		CreatePathWall(posx, posy + 2, posz-15, 0, vec3(1,0,0),2); //back
		CreatePathWall(posx+15, posy + 2, posz, 90, vec3(0, 1, 0),2); //left
	}
	else if (side == 2)
	{
		
		CreatePathWall(posx + 15, posy + 2, posz, 90, vec3(0, 1, 0), 2); //right
		CreatePathWall(posx, posy + 2, posz + 15, 0, vec3(1, 0, 0), 2); //back
	}
	else if (side == 3)
	{
		CreatePathWall(posx, posy + 2, posz + 15, 0, vec3(1, 0, 0), 2); //front
		CreatePathWall(posx-15, posy + 2, posz, 90, vec3(0, 1, 0), 2); //right
	}
	else if (side == 4)
	{
		CreatePathWall(posx, posy + 2, posz - 15, 0, vec3(1, 0, 0), 2); //back
		CreatePathWall(posx - 15, posy + 2, posz, 90, vec3(0, 1, 0), 2); //left
	}

	CreateCornerFloor(posx, posy, posz);
	
}


void ModuleSceneIntro::CreateRamp(int posx, int posy, int posz, int upordown)
{
	if (upordown == 1)
	{
		Cube* cube = cubeCreation(vec3(posx, posy + 8, posz), vec3(50, 1, 20), Grey, 20, vec3(0, 0, 1));
		cubePhysList.add(CreateCubePhysbody(cube, this));
		cubeList.add(cube);
	}
	else if (upordown == 2)
	{
		Cube* cube = cubeCreation(vec3(posx, posy + 8, posz), vec3(50, 1, 20), Grey, -20, vec3(0, 0, 1));
		cubePhysList.add(CreateCubePhysbody(cube, this));
		cubeList.add(cube);
	}
	
}