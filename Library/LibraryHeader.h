#pragma once

//必要なヘッダーファイルのインクルード
#include <windows.h>
#include <D3D10.h>
#include <d3dx10.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

//ウインドウ名
static WCHAR szAppName[] = L"ライブラリ";

//ウインドウサイズ
constexpr int WINDOW_WIDTH{ 640 };
constexpr int WINDOW_HEIGHT{ 480 };

//フレームレート
constexpr int FRAME_RATE{ 60 };

//DirectX
#define DX CDirectX::GetInstance()

//初めのシーン
#define START_SCENE SceneName::SceneMain 