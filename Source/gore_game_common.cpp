#include "gore_game_common.h"

void GAMEUpdateCameraVectors(
	game_camera* Camera,
	float DeltaPitch,
	float DeltaYaw,
	float DeltaRoll,
	v3 WorldUp)
{

	float LockEdge = 89.0f * DEG_TO_RAD;

	Camera->Pitch += DeltaPitch;
	Camera->Yaw += DeltaYaw;
	Camera->Roll += DeltaRoll;
	
	Camera->Pitch = Clamp(Camera->Pitch, -LockEdge, LockEdge);

	Camera->Front.x = Cos(Camera->Yaw) * Cos(Camera->Pitch);
	Camera->Front.y = Sin(Camera->Pitch);
	Camera->Front.z = Sin(Camera->Yaw) * Cos(Camera->Pitch);

	Camera->Front = Normalize(Camera->Front);

	Camera->Left = Normalize(Cross(WorldUp, Camera->Front));
	Camera->Up = Normalize(Cross(Camera->Front, Camera->Left));
}

game_camera_setup GAMECameraSetup(
	game_camera* Camera, 
	u32 Width,
	u32 Height, 
	u32 ProjectionType,
	float Far,
	float Near,
	float FieldOfView)
{
	game_camera_setup Setup = {};

	Setup.Camera = Camera;

	Setup.ProjectionMatrix = Identity();
	switch (ProjectionType) {
		case CameraProjection_Orthographic: {
			Setup.ProjectionMatrix = OrthographicProjection(Width, Height, Far, Near);
		}break;

		case CameraProjection_InfiniteOrthographic: {
			Setup.ProjectionMatrix = OrthographicProjection(Width, Height);
		}break;

		case CameraProjection_Perspective: {
			Setup.ProjectionMatrix = PerspectiveProjection(Width, Height, FieldOfView, Far, Near);
		}break;

		default: {
			InvalidCodePath;
		}break;
	}

	Setup.ViewMatrix = GAMEGetCameraTransform(Camera);
	Setup.ProjectionViewMatrix = Multiply(Setup.ProjectionMatrix, Setup.ViewMatrix);

	return(Setup);
}