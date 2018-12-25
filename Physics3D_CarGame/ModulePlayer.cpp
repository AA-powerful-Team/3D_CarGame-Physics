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

	addVehicle();


	RevEngineSound=App->audio->LoadFx("FX/Rev.wav");
	StartingEngine = App->audio->LoadFx("FX/StartingEngine.wav");
	EngineLoop = App->audio->LoadFx("FX/EngineLoop.wav");
	constantSpeedEngine = App->audio->LoadFx("FX/ConstantSpeed.wav");
	AcceleratingEngine = App->audio->LoadFx("FX/Accelerating.wav");
	BrakingSound = App->audio->LoadFx("FX/Braking.wav");
	Turning = App->audio->LoadFx("FX/turning.wav");
	TireSkid = App->audio->LoadFx("FX/tireSkid.wav");
	BoostSound = App->audio->LoadFx("FX/boost.wav");

	
	
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


	if (Menu == true) {
	
		if (playMusic == true) {
			App->audio->PlayMusic("FX/MenuGame.wav");
			playMusic = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

			Menu = false;
			playMusic = true;
			App->audio->PlayFx(StartingEngine);
			App->audio->PlayMusic("FX/StageMusic.wav");
		}
	}

	if (Menu == false) {



		if (Boost == true) {

			vehicle->ApplyEngineForce(BOOST_POWER);
			App->audio->PlayFx(BoostSound);
			
			if(BoostTime.Read()>2000)
			Boost = false;

		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && vehicle->GetKmh() < 5) {


			App->audio->PlayFx(RevEngineSound);

		}
		if (Boost == false) {

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE)
			{

				if (vehicle->GetKmh() > 0) {

					acceleration = -MAX_ACCELERATION * 0.25;
				}


			}
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			acceleration = MAX_ACCELERATION;

			if (EngineAcceleratingSoundTimer.Read() >= 4000 && vehicle->GetKmh() > 100) {
				App->audio->PlayFx(AcceleratingEngine, 0, 4);
				EngineAcceleratingSoundTimer.Start();

			}

		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{

			acceleration = -MAX_ACCELERATION;

		}
		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) {


			brake = BRAKE_POWER;


		}

		//playing Sound Conditions

		MusicConditions();

		

		vehicle->ApplyEngineForce(acceleration);
		vehicle->Turn(turn);
		vehicle->Brake(brake);


		vehicle->Render();


		char title[300];
		sprintf_s(title, "%.1f Km/h Lap: %i /3 TotalTime:%i ", vehicle->GetKmh(), App->scene_intro->laps, App->scene_intro->total_Time);
		App->window->SetTitle(title);


		App->camera->LookAt(vehicle->GetVehiclePos());


		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_IDLE)
		{


			if (CurrentVelocity > vehicle->GetKmh()) {

				if (CameraZoom > 10)
					CameraZoom -= cameraAcceleration;

				App->camera->Position = (vehicle->GetVehiclePos() - vehicle->GetDirectionVec() * CameraZoom + vec3(0, 6, 0));
			}
			else if (CurrentVelocity < vehicle->GetKmh()) {


				if (vehicle->GetKmh() > 50 && CameraZoom < 20)
					CameraZoom += cameraAcceleration;

				App->camera->Position = (vehicle->GetVehiclePos() - vehicle->GetDirectionVec() * CameraZoom + vec3(0, 6, 0));


			}
			else if (CurrentVelocity == vehicle->GetKmh()) {

				if (CurrentVelocity < 2.00 && CameraZoom > 10) {

					CameraZoom -= 0.5;
				}
				App->camera->Position = (vehicle->GetVehiclePos() - vehicle->GetDirectionVec() * CameraZoom + vec3(0, 6, 0));
			}

		}
		else if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {


			App->camera->Position = (vehicle->GetVehiclePos() + vehicle->GetDirectionVec() * 12 + vec3(0, 6, 0));

		}

		CurrentVelocity = vehicle->GetKmh();

		
	}
	else {


	brake = BRAKE_POWER;
	vehicle->Brake(brake);

	App->camera->LookAt(vehicle->GetVehiclePos());

	App->camera->Position = (vehicle->GetVehiclePos() - vehicle->GetDirectionVec() *CameraZoom + vec3(0,500, -20));

	char title[100];
	sprintf_s(title, "PRESS LETTER E TO START ");
	App->window->SetTitle(title);

	}


	return UPDATE_CONTINUE;

}



void ModulePlayer::MusicConditions() {


	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_UP) {


		App->audio->StopChannel(1, 300);


	}

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_DOWN && vehicle->GetKmh() > 50) {


		App->audio->PlayFx(BrakingSound, 0, 1);


	}

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)) {



		App->audio->PlayFx(Turning);


	}

	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) && vehicle->GetKmh() > 170) {



		App->audio->PlayFx(TireSkid);


	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP) {



		App->audio->StopChannel(4, 500);
		EngineAcceleratingSoundTimer.Start();


	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE && vehicle->GetKmh() < 10) {

		if (EngineSoundLoopTimer.Read() >= 1000) {
			App->audio->PlayFx(EngineLoop);
			EngineSoundLoopTimer.Start();
		}

	}

	if (vehicle->GetKmh() > 10) {

		if (EngineConstantSpeedTimer.Read() >= 4000) {
			App->audio->PlayFx(constantSpeedEngine, 0, 3);
			EngineConstantSpeedTimer.Start();
		}
	}
	else if (vehicle->GetKmh() <= 10) {

		App->audio->StopChannel(3, 500);

	}






}

void ModulePlayer::resetPlayerPos()
{
	//maybe destroy and create vehicle? to reset forces

	vehicle->SetPos(100, 5, -165); //start point
	vehicle->GetTransform(&matrix);
	matrix.rotate(180, vec3(0, 1, 0));
	vehicle->SetTransform(&matrix);
	
	
}

void ModulePlayer::resetOrientationPos()
{
	vec3 aux;
	vehicle->GetTransform(&matrix);
	matrix.rotate(360, vec3(1, 0, 0));
	
	matrix.rotate(180, vec3(0, 1, 0));
	vehicle->SetTransform(&matrix);
	
	aux = vehicle->GetVehiclePos();
	vehicle->SetPos(aux.x,aux.y+3,aux.z);
}

void ModulePlayer::addVehicle()
{
	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 5);
	car.chassis_offset.Set(0, 0.70, 0);

	car.chassis2_size.Set(2, 1, 1.50);
	car.chassis2_offset.Set(0, 1, 1.75);

	car.chassis3_size.Set(1.25, 1, 1);
	car.chassis3_offset.Set(0, 0.70, 2.50);

	car.chassis4_size.Set(2, 1, 1.5);
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

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

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
	car.wheels[0].frontWheel = true;
	car.wheels[0].SkidWheel = false;
	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = false;
	car.wheels[1].brake = true;
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
	vehicle->SetPos(100, 2, -165); //start point
								   //vehicle->SetPos(126, 5, 50);
	vehicle->GetTransform(&matrix);
	matrix.rotate(180, vec3(0, 1, 0));
	vehicle->SetTransform(&matrix);
}
