#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
//#include "PhysVehicle3D.h"

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
	laps = 0;
	Lap1 = 0;
	Lap2 = 0;
	Lap3 = 0;

	lap1 = 0;
	lap2 = 0;
	lap3 = 0;
	total_Time = 0;

	
	//App->physics->AddConstraintHinge(*motor_1, *helix_1, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);
												
	//CreateSpeedBoost(420, 4, -350, 15, 1, 3, Yellow);

	restart = false;
	passLine = false;
	lock = false;


	createObstacle(100, 4, -180, 25, 1, 1, Blue, TypeObject::START, true, true);
	
	Laptime.Start();
	Totaltime.Start();

	// test
	//
	//fan1 = SpinMachine(160, 7, 50, 10, 1, 1, 90); //lo que gira
	//mover1 = TorqueMove(160, 7, 50, 90); // la base
	//App->physics->AddConstraintHinge(*mover1.Pcubeinfo, *fan1.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);

	fan2 = SpinMachine(160, 4, 50, 10, 1, 1, 90,100); //lo que gira
	mover2 = TorqueMove(160, 4, 50, 90,100); // la base
	App->physics->AddConstraintHinge(*mover2.Pcubeinfo, *fan2.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, false);


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

	total_Time = (int)Totaltime.ReadSec();

	p2List_item <Cube*>* cube_render = cubeList.getFirst();
	while (cube_render != nullptr) {
		
		cube_render->data->Render();
		cube_render = cube_render->next;
	
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP && App->player->vehicle->overturned() == true)
	{
		App->player->resetOrientationPos();
	}


	//if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	//{

	//	if (laps == 1)
	//		Lap1 = Laptime.ReadSec();
	//	else if (laps == 2)
	//		Lap2 = Laptime.ReadSec();
	//	else if (laps == 3)
	//		Lap3 = Laptime.ReadSec();
	//	else if (laps == 4)
	//		restart = true;
	//	/*else if (laps==0)
	//	laps = 1;*/

	//	laps++;
	//	Laptime.Start();
	//}
	if (lock == true && BlockT.ReadSec() >= 30)
	{
		lock = false;
	}

	if (passLine && lock == false)
	{

		if (laps == 1)
		{
			Lap1 = Laptime.ReadSec();
			lap1 = (int)Lap1;

		}	
		else if (laps == 2)
		{
			Lap2 = Laptime.ReadSec();
			lap2 = (int)Lap2;

		}	
		else if (laps == 3)
		{
			Lap3 = Laptime.ReadSec(); 
			lap3 = (int)Lap3;
			restart = true;
		}
		
		lock = true;

		laps++;
		Laptime.Start();
		passLine = false;
		BlockT.Start();
	}
	
	

	if (restart)
	{
		
		App->player->resetPlayerPos();

		if (laps <= 4)
		{
			//win condition stop movement. postion camera active and inactive bool

		}

		laps = 0;

		Lap1 = 0;
		Lap2 = 0;
		Lap3 = 0;

		lap1 = 0;
		lap2 = 0;
		lap3 = 0;

		Totaltime.Start();
		restart = false;
	}

	
	fan2.Pcubeinfo->GetTransform(&fan2.cubeinfo->transform);
	fan2.cubeinfo->Render();






	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->type==TypeObject::BOOST_SPEED) {

		App->player->Boost=true;

	}
	else if (body1->type == TypeObject::OUT_OF_BOUNDS && body1->type == TypeObject::VEHICLE)
	{
		restart = true;
	}
	else if (body1->type == TypeObject::START)
	{
		passLine = true;
	}



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

PhysBody3D* ModuleSceneIntro::CreateCubePhysbody(Cube* cube, Module* Callback, TypeObject type,bool is_sensor,float mass) {

	PhysBody3D* cubeP;
	cubeP = App->physics->AddBody(*cube, mass);
	cubeP->type = type;
	cubeP->AsSensor(is_sensor);
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

void ModuleSceneIntro::createObstacle(int posx, int posy, int posz, int sizex, int sizey, int sizez, Color color, TypeObject type , bool is_sensor, bool wire)
{
	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(sizex, sizey, sizez), color, 0, vec3(0, 0, 1));
	cube->wire = wire;
	cubePhysList.add(CreateCubePhysbody(cube, this, type, is_sensor));
	cubeList.add(cube);

}

void ModuleSceneIntro::setObstacle()
{
	createObstacle(420, 4, -310, 2, 5, 60, Red);
	createObstacle(425, 4, -230, 2, 5, 60, Red);
	createObstacle(415, 4, -230, 2, 5, 60, Red);
	createObstacle(415, 4, -140, 10, 5, 10, Red);
	createObstacle(425, 4, -100, 10, 5, 10, Red);
	//createObstacle(420, 4, -10, 1, 1, 1, Blue); // obstaculo palanca sin torque

	createObstacle(360, 4, 55, 5, 5, 5, Red);
	createObstacle(280, 4, 50, 5, 5, 5, Red);

	
	//(160, 5, 50, 1, 1, 1, Blue);  // obstaculo palanca con torque
	//createObstacle(60, 5, 50, 1, 1, 1, Blue);   // obstaculo palanca con torque
	//createObstacle(-40, 5, 50, 1, 1, 1, Blue);  // obstaculo palanca con torque
	//createObstacle(-140, 5, 50, 1, 1, 1, Blue); // obstaculo palanca con torque

	//createObstacle(320, 5, -110, 1, 1, 1, Blue); // obstaculo palanca sin torque
	//createObstacle(260, 5, -50, 1, 1, 1, Blue);  // obstaculo palanca con torque
	//createObstacle(160, 5, -50, 1, 1, 1, Blue);  // obstaculo palanca con torque

	//boost
	CreateSpeedBoost(420, 4, -350, 15, 1, 1, Yellow);
	CreateSpeedBoost(390, 4, 50, 1, 1, 15, Yellow);
	CreateSpeedBoost(-190, 5, -170, 1, 2, 15, Yellow);
}

void ModuleSceneIntro::CreateWorldBoundaries()
{

	createObstacle(-140, 0, 100, 1300, 1, 1200, OtherGrey, TypeObject::OUT_OF_BOUNDS,true);
	createObstacle(500, 90, 170, 2, 250, 1400, LightBlue, TypeObject::OUT_OF_BOUNDS, true);
	createObstacle(-500, 90, 170, 2, 250, 1400, LightBlue, TypeObject::OUT_OF_BOUNDS, true);
	createObstacle(100, 90, 200, 1400, 800, 200, LightBlue, TypeObject::OUT_OF_BOUNDS, true);
	createObstacle(100, 90, -550, 1400, 800, 200, LightBlue, TypeObject::OUT_OF_BOUNDS, true);

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

void ModuleSceneIntro::CreateSpeedBoost(int posx, int posy, int posz, int sizex, int sizey, int sizez, Color color) {

	Cube*Boost = cubeCreation(vec3(posx, posy, posz),vec3(sizex, sizey, sizez),color, 0, vec3(0, 0, 1));
	cubePhysList.add(CreateCubePhysbody(Boost, this,TypeObject::BOOST_SPEED,true));
	cubeList.add(Boost);

}
void ModuleSceneIntro::CreateTurbine(int posTorqx, int posTorqy, int sizeTorqz, int sizeShowx, int sizeShowy, int sizeShowz ,Color colorTorq) {

	//Cube*Pivot = cubeCreation(vec3(posTorqx, posTorqy, 1), vec3(1, 1, sizeTorqz), colorTorq, 0, vec3(0, 0, 1));
	//PhysBody3D*Pbody = CreateCubePhysbody(Pivot, this, TypeObject::NONE,false,1000.0f);
	//
	////Pivot.SetRotation(90, vec3(0, 1, 0));
	//
	////Pbody->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	//cubePhysList.add(Pbody);
	//cubeList.add(Pivot);

	//
	//Cube* Shovel = cubeCreation(vec3((posTorqx+(1/2)), posTorqy, (1-(sizeTorqz/2))+ sizeShowx/2), vec3(sizeShowx, sizeShowy, sizeShowz), Green, 0, vec3(0, 0, 1));
	//PhysBody3D*Sbody = CreateCubePhysbody(Shovel, this, TypeObject::TURBINE,false, 1000.0f);
	//cubePhysList.add(Sbody);
	//cubeList.add(Shovel);

	//vec3 piv(posTorqx + 0.5,posTorqy,1 - (sizeTorqz /2));
	//vec3 neut(0,0,0);

	//App->physics->AddConstraintHinge(*Pbody,*Sbody, piv, piv, vec3(0, 1, 0), vec3(0, 0, 0),true);

}

compact_info ModuleSceneIntro::SpinMachine( int posx, int posy, int posz, int sizex, int sizey, int sizez, float angle, float mass)
{
	Cube* cube= cubeCreation(vec3(posx, posy, posz), vec3(sizex, sizey, sizez), Red, angle, vec3(0, 1, 0));
	cubeList.add(cube);
	
	PhysBody3D* attacher;
	attacher = CreateCubePhysbody(cube, this, TypeObject::NONE,false, mass);
	cubePhysList.add(attacher);
	attacher->PointerGet()->setLinearFactor(btVector3(0, 0, 0));

	compact_info aux;
	aux.cubeinfo = cube;
	aux.Pcubeinfo = attacher;

	return aux;
}

compact_info ModuleSceneIntro::TorqueMove(int posx, int posy, int posz, float angle,float mass) {

	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(1, 1, 1), Red, angle, vec3(0, 1, 0));
	cubeList.add(cube);

	PhysBody3D* attached;
	attached = CreateCubePhysbody(cube, this, TypeObject::NONE, false, mass);
	cubePhysList.add(attached);
	attached->PointerGet()->setLinearFactor(btVector3(0, 0, 0));

	compact_info aux;
	aux.cubeinfo = cube;
	aux.Pcubeinfo = attached;

	return aux;
}