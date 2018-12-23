#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "Timer.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 5);
	car.chassis_offset.Set(0,0.70, 0);

	car.chassis2_size.Set(2,1,1.50);
	car.chassis2_offset.Set(0, 1,1.75);
	
	car.chassis3_size.Set(1.25, 1, 1);
	car.chassis3_offset.Set(0,0.70,2.50);

	car.chassis4_size.Set(2,1,1.5);
	car.chassis4_offset.Set(0, 1.5, 1.25);

	car.chassis5_size.Set(1.75, 1.10, 0.5);
	car.chassis5_offset.Set(0, 0.85, 3);

	car.chassis6_size.Set(1, 1.5, 4);
	car.chassis6_offset.Set(0, 1, 0);


	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 1000.0f;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------

	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.0f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = false;
	car.wheels[0].brake = true;
	car.wheels[0].steering = true;
	car.wheels[0].frontWheel=true;
	car.wheels[0].SkidWheel = false;
	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive =false;
	car.wheels[1].brake =true;
	car.wheels[1].steering = true;
	car.wheels[1].frontWheel = true;
	car.wheels[1].SkidWheel = false;
	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = true;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;
	car.wheels[2].frontWheel = false;
	car.wheels[2].SkidWheel = true;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = true;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;
	car.wheels[3].frontWheel = false;
	car.wheels[3].SkidWheel = true;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 12, 10);


	RevEngineSound=App->audio->LoadFx("FX/Rev.wav");
	StartingEngine = App->audio->LoadFx("FX/StartingEngine.wav");
	EngineLoop = App->audio->LoadFx("FX/EngineLoop.wav");
	constantSpeedEngine = App->audio->LoadFx("FX/ConstantSpeed.wav");
	AcceleratingEngine = App->audio->LoadFx("FX/Accelerating.wav");

	App->audio->PlayFx(StartingEngine);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	


	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;



	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && vehicle->GetKmh()<5) {


		App->audio->PlayFx(RevEngineSound);

	}
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{

		acceleration = -MAX_ACCELERATION;
	
	}
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) {


		brake = BRAKE_POWER;


	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {


		skid = SKID_POWER;


	}

	//playing Sound Conditions
	
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE && 
		App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE && 
		vehicle->GetKmh()<10){
	
		if (EngineSoundLoopTimer.Read()>=1000) {
			App->audio->PlayFx(EngineLoop);
			EngineSoundLoopTimer.Start();
		}


	}

	if (vehicle->GetKmh() >10) {

		if (EngineConstantSpeedTimer.Read() >= 4000) {
			App->audio->PlayFx(constantSpeedEngine,0, 3);
			EngineConstantSpeedTimer.Start();
		}
	}
	else if (vehicle->GetKmh()<=10) {

		App->audio->StopChannel(3,500);

	}
	if (constantVelocity == false && vehicle->GetKmh()>75) {

	/*	if (EngineAcceleratingSoundTimer.Read()>=1000) {
			App->audio->PlayFx(AcceleratingEngine,1,2);
			EngineAcceleratingSoundTimer.Start();
		}*/
	}
	else if (constantVelocity == true){

		App->audio->StopChannel(2,500);

	}


	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	//vehicle->Skid(skid);

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	App->camera->LookAt(vehicle->GetVehiclePos());


	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_IDLE)
	{


		if (CurrentVelocity > vehicle->GetKmh()) {
			
			if(CameraZoom>10)
			CameraZoom -= cameraAcceleration;

			App->camera->Position = (vehicle->GetVehiclePos() - vehicle->GetDirectionVec() * CameraZoom + vec3(0, 6, 0));
		}
		else if (CurrentVelocity < vehicle->GetKmh()) {
			
			
			if (vehicle->GetKmh()>50 && CameraZoom < 20)
			CameraZoom += cameraAcceleration;

				App->camera->Position = (vehicle->GetVehiclePos() - vehicle->GetDirectionVec() * CameraZoom + vec3(0, 6, 0));

				
		}
		else if (CurrentVelocity == vehicle->GetKmh()) {
			
			if (CurrentVelocity < 2.00 && CameraZoom > 10) {

				CameraZoom -=0.5;
			}
			App->camera->Position = (vehicle->GetVehiclePos() - vehicle->GetDirectionVec() * CameraZoom + vec3(0, 6, 0));
		}

	}
	else if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
	
	
		App->camera->Position = (vehicle->GetVehiclePos() + vehicle->GetDirectionVec() * 12 + vec3(0, 6, 0));

	}

	CurrentVelocity = vehicle->GetKmh();

	return UPDATE_CONTINUE;
}



