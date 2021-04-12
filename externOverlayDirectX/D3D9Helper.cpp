#include "D3D9Helper.h"

/*
D3D9Helper::D3D9Helper() {}

//may need to check out CasualGamer's video on what this function does. Otherwise not too important
bool D3D9Helper::initVTable()
{
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION); // create IDirect3D9 object
    if (!pD3D) {
        return false;
    }

    D3DPRESENT_PARAMETERS d3dparams = { 0 };
    d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dparams.hDeviceWindow = GetForegroundWindow();
    d3dparams.Windowed = true;

    IDirect3DDevice9* pDevice;
    HRESULT result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dparams.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dparams, &pDevice);
    if (FAILED(result) || !pDevice) {
        pD3D->Release();
        return false;
    }
    //if device creation worked out -> lets get the virtual table:
    this->vTable = *reinterpret_cast<void***>(pDevice);

    pD3D->Release();
    pDevice->Release();
    return true;
}

void D3D9Helper::drawText(const char* text, float x, float y, D3DCOLOR color)
{
    RECT fontPos;
    fontPos.left = x;
    fontPos.top = y;
    if (!pTextFont)
        D3DXCreateFont(pDevice, 25, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Comic Sans MS", &pTextFont);
    pTextFont->DrawTextA(0, text, strlen(text), &fontPos, DT_NOCLIP, color);
}
void D3D9Helper::drawText(const char* text, float x, float y, int a, int r, int g, int b) {
    drawText(text, x, y, D3DCOLOR_ARGB(a, r, g, b));
}

void D3D9Helper::drawText(std::string textParam, float x, float y, D3DCOLOR color) {
    const char* text = textParam.c_str();
    drawText(text, x, y, color);
}

void D3D9Helper::drawFilledRectangle(float x, float y, float width, float height, D3DCOLOR color) {
    D3DXVECTOR2 vertices[2] = { D3DXVECTOR2(x + width / 2, y),D3DXVECTOR2(x + width / 2, y + height) };
    if (!line) 
        D3DXCreateLine(pDevice, &line);
    line->SetWidth(width);
    line->Draw(vertices, 2, color);
}

void D3D9Helper::drawRectangle(float x, float y, float width, float height, D3DCOLOR color) {
    D3DXVECTOR2 rect[5];
    rect[0] = D3DXVECTOR2(x, y);
    rect[1] = D3DXVECTOR2(x + width, y);
    rect[2] = D3DXVECTOR2(x + width, y + height);
    rect[3] = D3DXVECTOR2(x, y + height);
    rect[4] = D3DXVECTOR2(x, y);
    if (!line) {
        D3DXCreateLine(pDevice, &line);
    }
    line->SetWidth(1);
    line->Draw(rect, 5, color);
}
*/