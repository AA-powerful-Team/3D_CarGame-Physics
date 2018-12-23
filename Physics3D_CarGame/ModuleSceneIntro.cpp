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

	createMap();

	App->audio->PlayMusic("FX/StageMusic.wav");

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
		Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(20, 3, 1), DarkGrey, angle, rotation);
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
	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(20, 1, 20), Grey, 0, vec3(1,0,0));
	cubePhysList.add(CreateCubePhysbody(cube, this));
	cubeList.add(cube);
}


void ModuleSceneIntro::CreateCorner(int posx, int posy, int posz, int side)
{

	if (side == 1)
	{
		CreatePathWall(posx, posy + 2, posz-10, 0, vec3(1,0,0),2); //back
		CreatePathWall(posx+10, posy + 2, posz, 90, vec3(0, 1, 0),2); //left
	}
	else if (side == 2)
	{
		
		CreatePathWall(posx + 10, posy + 2, posz, 90, vec3(0, 1, 0), 2); //right
		CreatePathWall(posx, posy + 2, posz + 10, 0, vec3(1, 0, 0), 2); //back
	}
	else if (side == 3)
	{
		CreatePathWall(posx, posy + 2, posz + 10, 0, vec3(1, 0, 0), 2); //front
		CreatePathWall(posx-10, posy + 2, posz, 90, vec3(0, 1, 0), 2); //right
	}
	else if (side == 4)
	{
		CreatePathWall(posx, posy + 2, posz - 10, 0, vec3(1, 0, 0), 2); //back
		CreatePathWall(posx - 10, posy + 2, posz, 90, vec3(0, 1, 0), 2); //left
	}

	CreateCornerFloor(posx, posy, posz);
	
}


void ModuleSceneIntro::CreateRamp(int posx, int posy, int posz, int upordown)
{
	if (upordown == 1)
	{
		Cube* cube = cubeCreation(vec3(posx, posy + 8, posz), vec3(100, 1, 20), Grey, 20, vec3(0, 0, 1));
		cubePhysList.add(CreateCubePhysbody(cube, this));
		cubeList.add(cube);
	}
	else if (upordown == 2)
	{
		Cube* cube = cubeCreation(vec3(posx, posy + 8, posz), vec3(100, 1, 20), Grey, -15, vec3(0, 0, 1));
		cubePhysList.add(CreateCubePhysbody(cube, this));
		cubeList.add(cube);
	}
	
}

void ModuleSceneIntro::createMap() 
{
	CreateCorner(420, 2, -370, 1);
	CreateRoad(420, 2, -310, 90, vec3(0, 1, 0));
	CreateRoad(420, 2, -210, 90, vec3(0, 1, 0));
	CreateRoad(420, 2, -110, 90, vec3(0, 1, 0));
	CreateRoad(420, 2, -10, 90, vec3(0, 1, 0));
	CreateCorner(420, 2, 50, 2);

	CreateRoad(360, 2, 50, 0, vec3(0, 1, 0));
	CreateRoad(260, 2, 50, 0, vec3(0, 1, 0));
	CreateRoad(160, 2, 50, 0, vec3(0, 1, 0));
	CreateRoad(60, 2, 50, 0, vec3(0, 1, 0));
	CreateRoad(-40, 2, 50, 0, vec3(0, 1, 0));
	CreateRoad(-140, 2, 50, 0, vec3(0, 1, 0));
	CreateCorner(-200, 2, 50, 3);

	CreateRoad(-200, 2, -10, 90, vec3(0, 1, 0));
	CreateRoad(-200, 2, -110, 90, vec3(0, 1, 0));
	CreateCorner(-200, 2, -170, 4);

	CreateRamp(-143, 11, -170, 1);
	CreateRamp(-57, 42.6, -170, 1);

	CreateRamp(89, 32, -170, 2);
	CreateRamp(186, 6, -170, 2);
	CreateRoad(260, 2, -170, 0, vec3(0, 1, 0));
	CreateCorner(320, 2, -170, 1);

	CreateRoad(320, 2, -110, 90, vec3(0, 1, 0));
	CreateCorner(320, 2, -50, 2);

	CreateRoad(260, 2, -50, 0, vec3(0, 1, 0));
	CreateRoad(160, 2, -50, 0, vec3(0, 1, 0));
	CreateCorner(100, 2, -50, 3);

	CreateRoad(100, 2, -110, 90, vec3(0, 1, 0));
	CreateRoad(100, 2, -210, 90, vec3(0, 1, 0));
	CreateRoad(100, 2, -310, 90, vec3(0, 1, 0));
	CreateCorner(100, 2, -370, 4);

	CreateRoad(160, 2, -370, 0, vec3(0, 1, 0));
	CreateRoad(260, 2, -370, 0, vec3(0, 1, 0));
	CreateRoad(360, 2, -370, 0, vec3(0, 1, 0));

	//start position
	createObstacle(111, 7, -180, 3, 20, 5, Green);
	createObstacle(89, 7, -180, 3, 20, 5, Green);
	createObstacle(100, 17, -180, 20, 5, 5, Green);

	// buildings
	createBuilding(-140, 25, -300, 200, 50, 140);
	createBuilding(-100, 30, -60, 160, 60, 160);
	createBuilding(260, 35, -280, 260, 70, 120);
	createBuilding(200, 10, -110, 170, 20, 60);
	
	// obstacles
	setObstacle();

	//limits
	CreateWorldBoundaries();

}

void ModuleSceneIntro::createObstacle(int posx, int posy, int posz, int sizex, int sizey, int sizez, Color color)
{
	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(sizex, sizey, sizez), color, 0, vec3(0, 0, 1));
	cubePhysList.add(CreateCubePhysbody(cube, this));
	cubeList.add(cube);

}

void ModuleSceneIntro::setObstacle()
{
	createObstacle(420, 4, -310, 2, 5, 60, Red);
	createObstacle(425, 4, -230, 2, 5, 60, Red);
	createObstacle(415, 4, -230, 2, 5, 60, Red);
	createObstacle(415, 4, -140, 10, 5, 10, Red);
	createObstacle(425, 4, -100, 10, 5, 10, Red);
	createObstacle(420, 4, -10, 1, 1, 1, Blue); // obstaculo palanca sin torque

	createObstacle(360, 4, 55, 5, 5, 5, Red);
	createObstacle(280, 4, 50, 5, 5, 5, Red);

	createObstacle(160, 5, 50, 1, 1, 1, Blue);  // obstaculo palanca con torque
	createObstacle(60, 5, 50, 1, 1, 1, Blue);   // obstaculo palanca con torque
	createObstacle(-40, 5, 50, 1, 1, 1, Blue);  // obstaculo palanca con torque
	createObstacle(-140, 5, 50, 1, 1, 1, Blue); // obstaculo palanca con torque

	createObstacle(320, 5, -110, 1, 1, 1, Blue); // obstaculo palanca sin torque
	createObstacle(260, 5, -50, 1, 1, 1, Blue);  // obstaculo palanca con torque
	createObstacle(160, 5, -50, 1, 1, 1, Blue);  // obstaculo palanca con torque
}

void ModuleSceneIntro::CreateWorldBoundaries()
{

	createObstacle(-140, 0, 100, 1300, 1, 1200, OtherGrey);
	createObstacle(500, 90, 170, 2, 250, 1400, LightBlue);
	createObstacle(-500, 90, 170, 2, 250, 1400, LightBlue);

	createObstacle(100, 90, 200, 1400, 800, 200, LightBlue);
	createObstacle(100, 90, -550, 1400, 800, 200, LightBlue);

}

void ModuleSceneIntro::createBuilding(int posx, int posy, int posz, int sizex, int sizey, int sizez)
{
	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(sizex, sizey, sizez), GreyBuilding, 0, vec3(0, 0, 1));
	cubePhysList.add(CreateCubePhysbody(cube, this));
	cubeList.add(cube);
	Cube* cube2 = cubeCreation(vec3(posx, posy+sizey, posz), vec3(sizex/1.2, sizey, sizez/2), GreyBuilding, 0, vec3(0, 0, 1));
	cubePhysList.add(CreateCubePhysbody(cube2, this));
	cubeList.add(cube2);
	Cube* cube3 = cubeCreation(vec3(posx, posy +sizey*2, posz), vec3(sizex /1.5, sizey, sizez / 4), GreyBuilding, 0, vec3(0, 0, 1));
	cubePhysList.add(CreateCubePhysbody(cube3, this));
	cubeList.add(cube3);

}