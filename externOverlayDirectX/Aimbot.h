#include <Windows.h>
#include <Math.h>
#include "mem.h"
#include "geom.h"
#include "CameraEx.h"

class Aimbot
{
public:
	const int WINDOWWIDTH = 1024;
	const int WINDOWHEIGHT = 720;
	float PI = 3.14159265358;

	HANDLE hProc = 0;
	uintptr_t* entlist = (uintptr_t*)(0x50f4f8);
	uintptr_t localPlayer = (uintptr_t)0x50F4F4;
	uintptr_t localPlayerDeref = 0;
	uintptr_t numOfPlayers = 0x50f500;

	//uintptr_t pitchAddr;
	//uintptr_t yawAddr;

	int numOfPlayersDeref;

	Aimbot(HANDLE hProc);
	vec3 GetLocalPlayerPos();
	vec3 GetEntity();
	void AimAt();
};

