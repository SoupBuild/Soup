#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

import Sample.DirectX;

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	D3D12HelloTriangle sample(1280, 720, L"D3D12 Hello Triangle");
	return Win32Application::Run(&sample, hInstance, nCmdShow);
}
