#include "Aimbot.h"

Aimbot::Aimbot(HANDLE hProc, int width, int height)
{
	this->hProc = hProc;
	WINDOWWIDTH = width;
	WINDOWHEIGHT = height;
	//Get local player's address
	ReadProcessMemory(hProc, (BYTE*)localPlayer, &(localPlayerDeref), sizeof(localPlayerDeref), 0);

	//Get number of players
	ReadProcessMemory(hProc, (BYTE*)numOfPlayers, &(numOfPlayersDeref), sizeof(numOfPlayersDeref), 0);

	//get pitch's address
	//pitchAddr = localPlayerDeref + 0x44;
	//ReadProcessMemory(hProc, (BYTE*)localPlayerDeref + 0x44, &(pitchAddr), sizeof(pitchAddr), 0);
	//get yaw's address
	//yawAddr = localPlayerDeref + 0x40;
	//ReadProcessMemory(hProc, (BYTE*)localPlayerDeref + 0x40, &(yawAddr), sizeof(yawAddr), 0);
}

vec3 Aimbot::GetLocalPlayerPos()
{
	uintptr_t currHeadPosXPtr = localPlayerDeref + 0x4;
	uintptr_t currHeadPosYPtr = localPlayerDeref + 0x8;
	uintptr_t currHeadPosZPtr = localPlayerDeref + 0xC;

	vec3 LPHeadPos = vec3();

	ReadProcessMemory(hProc, (BYTE*)currHeadPosXPtr, &(LPHeadPos.x), sizeof(LPHeadPos.x), 0);
	ReadProcessMemory(hProc, (BYTE*)currHeadPosYPtr, &(LPHeadPos.y), sizeof(LPHeadPos.y), 0);
	ReadProcessMemory(hProc, (BYTE*)currHeadPosZPtr, &(LPHeadPos.z), sizeof(LPHeadPos.z), 0);

	return LPHeadPos;

}
vec3 Aimbot::GetEntity()
{
	vec3 myPos = GetLocalPlayerPos();
	vec3 entOfLeastDist;

	uintptr_t currEntPtr;
	float leastDist = 0;


	// loop through entity list 
	// find the one with the least distance (relative to crosshair)
	// return his XYZ head pos
	//problems with this: 2) does not distinguish between allies and enemies
	for (int i = 0; i < numOfPlayersDeref; i++)
	{
		CameraEx cameraEx = CameraEx(hProc, WINDOWWIDTH, WINDOWHEIGHT);
		
		//currently this only gets the first entity in the entity list
		ReadProcessMemory(hProc, (BYTE*)entlist, &(currEntPtr), sizeof(currEntPtr), 0);
		//currEntPtr += 0x14;
		for (int j = 0; j < i; j++)
		{
			currEntPtr += 0x4;
		}

		ReadProcessMemory(hProc, (BYTE*)currEntPtr, &(currEntPtr), sizeof(currEntPtr), 0);

		uintptr_t XPosAddr = currEntPtr + 0x4;
		uintptr_t YPosAddr = currEntPtr + 0x8;
		uintptr_t ZPosAddr = currEntPtr + 0xC;

		Vector3 targetHead = Vector3();

		ReadProcessMemory(hProc, (BYTE*)XPosAddr, &(targetHead.x), sizeof(targetHead.x), 0);
		ReadProcessMemory(hProc, (BYTE*)YPosAddr, &(targetHead.y), sizeof(targetHead.y), 0);
		ReadProcessMemory(hProc, (BYTE*)ZPosAddr, &(targetHead.z), sizeof(targetHead.z), 0);

		//find the target's XY screen coordinates
		vec3 screenPos = cameraEx.WorldToScreen(targetHead);
		
		//... and my crosshair XY screen coordinates
		vec3 crossPos;

		crossPos.x = WINDOWWIDTH / 2;
		crossPos.y = WINDOWHEIGHT / 2;
		crossPos.z = 0.0f;
		
		//find the distance from my crosshair to the target
		float distRelCross = crossPos.Distance(screenPos);
		
		if (i == 0 || distRelCross < leastDist)
		{
			leastDist = distRelCross;
			entOfLeastDist = targetHead;
		}

	}

	return entOfLeastDist;

}
/*
vec3 Aimbot::GetEntity() 
{
	vec3 myPos = GetLocalPlayerPos();
	vec3 entOfLeastDist;

	uintptr_t currEntPtr;
	float leastDist = 0;
	
	
	// loop through entity list 
	// find the one with the least distance from me
	// return his XYZ head pos
	//2 problems with this: 1) it does not calculate how close he is to my crosshair 2) does not distinguish between allies and enemies
	for (int i = 0; i < numOfPlayersDeref; i++)
	{

		//currently this only gets the first entity in the entity list
		ReadProcessMemory(hProc, (BYTE*)entlist, &(currEntPtr), sizeof(currEntPtr), 0);
		//currEntPtr += 0x14;
		for (int j = 0; j < i; j++)
		{
			currEntPtr += 0x4;
		}

		ReadProcessMemory(hProc, (BYTE*)currEntPtr, &(currEntPtr), sizeof(currEntPtr), 0);

		uintptr_t XPosAddr = currEntPtr + 0x4;
		uintptr_t YPosAddr = currEntPtr + 0x8;
		uintptr_t ZPosAddr = currEntPtr + 0xC;

		Vector3 targetHead = Vector3();

		ReadProcessMemory(hProc, (BYTE*)XPosAddr, &(targetHead.x), sizeof(targetHead.x), 0);
		ReadProcessMemory(hProc, (BYTE*)YPosAddr, &(targetHead.y), sizeof(targetHead.y), 0);
		ReadProcessMemory(hProc, (BYTE*)ZPosAddr, &(targetHead.z), sizeof(targetHead.z), 0);

		float currDist = myPos.Distance(targetHead);
		if (i == 0 || currDist < leastDist)
		{
			leastDist = currDist;
			entOfLeastDist = targetHead;
		}

	}

	return entOfLeastDist;

}
*/
/*vec3 Aimbot::GetEntity()
{
	uintptr_t currEntPtr;

	//currently this only gets the first entity in the entity list
	ReadProcessMemory(hProc, (BYTE*)entlist, &(currEntPtr), sizeof(currEntPtr), 0);
	currEntPtr += 0x14;
	//for (int j = 0; j < i; j++)
	//{
	//	currEntPtr += 0x4;
	//}

	ReadProcessMemory(hProc, (BYTE*)currEntPtr, &(currEntPtr), sizeof(currEntPtr), 0);

	uintptr_t XPosAddr = currEntPtr + 0x4;
	uintptr_t YPosAddr = currEntPtr + 0x8;
	uintptr_t ZPosAddr = currEntPtr + 0xC;

	Vector3 targetHead = Vector3();

	ReadProcessMemory(hProc, (BYTE*)XPosAddr, &(targetHead.x), sizeof(targetHead.x), 0);
	ReadProcessMemory(hProc, (BYTE*)YPosAddr, &(targetHead.y), sizeof(targetHead.y), 0);
	ReadProcessMemory(hProc, (BYTE*)ZPosAddr, &(targetHead.z), sizeof(targetHead.z), 0);

	return targetHead;

}*/
void Aimbot::AimAt()
{
	CameraEx cameraEx = CameraEx(hProc, WINDOWWIDTH, WINDOWHEIGHT);
	
	vec3 target = GetEntity();

	//get center mass of player/entity (relative to head position)
	//target.z = target.z - 2.0f;

	vec3 point = cameraEx.WorldToScreen(target);

	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE;
	Input.mi.dx = point.x - WINDOWWIDTH / 2; 
	Input.mi.dy = point.y - WINDOWHEIGHT / 2;
	SendInput(1, &Input, sizeof(INPUT));
}
/*
void Aimbot::AimAt()
{
	vec3 target = GetEntity();

	CameraEx cameraEx = CameraEx(hProc);

	float aX, aY;

	vec3 screen = cameraEx.WorldToScreen(target);
	//if (screen == {0, 0, 0})
	//	return;

	//float centerX = pRefdef->width / 2.f;
	//float centerY = pRefdef->height / 2.f;

	float enemyX = screen.x;
	float enemyY = screen.y;

	//aX = enemyX - centerX;
	//aY = enemyY - centerY;

	//if (bSmoothAim)
	//{
	//	aX /= fSmoothAmount;
	//	aY /= fSmoothAmount;
	//}

	mouse_event(MOUSEEVENTF_MOVE, -enemyX, enemyY, 0, 0);
}*/
/*void Aimbot::AimAt()
{
	vec3 myPos = GetLocalPlayerPos();
	vec3 target = GetEntity();
	//get the pitch and yaw

	vec3 deltaVec = { target.x - myPos.x, target.y - myPos.y, target.z - myPos.z };
	float deltaVecLength = sqrtf(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);
	

	if (deltaVecLength < 300 && deltaVec.x != 0) 
	{
		float newPitch = asinf(deltaVec.z / deltaVecLength) * (180.0f / PI);
		float newYaw = atan2f(-deltaVec.x, deltaVec.y) * (180.0f / PI) + 180.0f;

		if (newPitch >= -89.0f && newPitch <= 89.0f)
		{
			//&& newYaw > 0.0f && newYaw < 360.0f
			mem::PatchEx((BYTE*)pitchAddr, (BYTE*)&newPitch, sizeof(newPitch), hProc);
			mem::PatchEx((BYTE*)yawAddr, (BYTE*)&newYaw, sizeof(newYaw), hProc);
		}
		//mem::PatchEx((BYTE*)pitchAddr, (BYTE*)&newPitch, sizeof(newPitch), hProc);
		//WriteProcessMemory(hProc, (BYTE*)pitchAddr, (BYTE*)&newPitch, sizeof(newPitch), nullptr);
	}

	//float newYaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);
	//if (newYaw < 0)
	//{}

	//set the new pitch and yaw values in game memory
	//uintptr_t newPitchPtr = newPitch;
	//uintptr_t newYawPtr = newYaw;

	//WriteProcessMemory(hProc, (BYTE*)pitchAddr, (BYTE*)&newPitch, sizeof(newPitch), nullptr);
	//WriteProcessMemory(hProc, (BYTE*)yawAddr, (BYTE*)&newYaw, sizeof(newYaw), nullptr);
}*/