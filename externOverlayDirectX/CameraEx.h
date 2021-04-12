#pragma once

#include "Camera.h"
#include <d3dx9math.h>

//find the camera and the resolution ptr path and store here before compile time
//const uintptr_t pCamera = 0x501AE8;
const uintptr_t pResolution = 0x510C94;
const uintptr_t matrixStart = 0x501AE8;
const uintptr_t localPlayer = (uintptr_t)0x50F4F4;
//const uintptr_t numOfPlayers = 0x50f500;
//int numOfPlayers = 6;

class CameraEx
{
public:
	float windowWidth = 0, windowHeight = 0;
	//Camera* camera;
	float matrix[16];

	HANDLE hProc = 0;

	CameraEx(HANDLE hProc);

	vec3 WorldToScreen(vec3 pos);
};