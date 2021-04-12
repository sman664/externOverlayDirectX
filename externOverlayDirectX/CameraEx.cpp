#include <algorithm>
#include "CameraEx.h"

//these offsets change with each game. (See CameraEx.h as well)
CameraEx::CameraEx(HANDLE hProc)
{
	windowWidth = 1024;
	windowHeight = 720;
	//adjust your resolution of game here
	//windowHeight = *(__int32*)pResolution;
	//windowWidth = *(__int32*)(pResolution + 0x4);
	this->hProc = hProc;
	
	float value;
	uintptr_t floatAddr;
	uintptr_t currMatrixIndex = 0;
	//uintptr_t myMatrixDeref;
	//ReadProcessMemory(hProc, (BYTE*)matrix, &(myMatrixDeref), sizeof(myMatrixDeref), 0);

	//initialize the view matrix in this class... usually 16 float values all right next to each other in memory;
	for (int m = 0; m < 16; m++)
	{
		value = 0;
		floatAddr = 0;
		floatAddr = matrixStart + currMatrixIndex;
		ReadProcessMemory(hProc, (BYTE*)floatAddr, &(value), sizeof(value), 0);

		matrix[m] = value;
		currMatrixIndex += 0x4;
	}

}
vec3 CameraEx::WorldToScreen(vec3 pos)
{
	//calculations that require view matrix, our position, and an enemy xyz position
	vec3 screen;
	
	Vec4 clipCoords;

	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
	{
		return screen;
	}

	vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	
	//Now we calculate the distance from us to object (vector functions localPlayer.Distance(object))
	
	//start by finding local player's address and getting our position...
	uintptr_t localPlayerDeref;
	ReadProcessMemory(hProc, (BYTE*)localPlayer, &(localPlayerDeref), sizeof(localPlayerDeref), 0);

	uintptr_t currHeadPosXPtr = localPlayerDeref + 0x4;
	uintptr_t currHeadPosYPtr = localPlayerDeref + 0x8;
	uintptr_t currHeadPosZPtr = localPlayerDeref + 0xC;

	vec3 locPlayerPos = vec3();

	ReadProcessMemory(hProc, (BYTE*)currHeadPosXPtr, &(locPlayerPos.x), sizeof(locPlayerPos.x), 0);
	ReadProcessMemory(hProc, (BYTE*)currHeadPosYPtr, &(locPlayerPos.y), sizeof(locPlayerPos.y), 0);
	ReadProcessMemory(hProc, (BYTE*)currHeadPosZPtr, &(locPlayerPos.z), sizeof(locPlayerPos.z), 0);

	//then by getting the enemy entity's position which is passed as vec3 already
	//Then calculate distance from us and enemy and store in our vec3
	screen.z = locPlayerPos.Distance(pos);

	//returns the screenCoords
	return screen;
}

//this was casualGamer's approach of WorldToScreen function for Halo d3dx game. But his view matrix was different and wierd...
/*
Vector3 CameraEx::WorldToScreen(Vector3 worldPosition)
{
	float yFOV = camera->fovx / windowWidth * windowHeight;
	D3DXVECTOR3 camToObj = { worldPosition.x - camera->position.x, worldPosition.y - camera->position.y, worldPosition.z - camera->position.z };
	float distToObj = sqrtf(camToObj.x * camToObj.x + camToObj.y * camToObj.y + camToObj.z * camToObj.z);
	D3DXVec3Normalize(&camToObj, &camToObj);

	float camYaw = atan2f(camera->lookAt.y, camera->lookAt.x);
	float objYaw = atan2f(camToObj.y, camToObj.x);
	float relYaw = camYaw - objYaw;

	if (relYaw > D3DX_PI)
		relYaw -= 2 * D3DX_PI;
	if (relYaw < D3DX_PI)
		relYaw += 2 * D3DX_PI;

	float objPitch = asin(camToObj.z);
	float camPitch = asin(camera->lookAt.z);
	float relPitch = camPitch - objPitch;

	float x = relYaw / (0.5 * camera->fovx);
	float y = relPitch / (0.5 * yFOV);

	x = (x + 1) / 2;
	y = (y + 1) / 2;

	return { x * windowWidth, y * windowHeight, distToObj };
}
*/