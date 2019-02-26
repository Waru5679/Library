#pragma once

//必要なヘッダーファイルのインクルード
#include <windows.h>
#include <D3D10.h>
#include <d3dx10.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

//定数
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//シーン読み込み
void LoadScene();

//ウインドウ名
static WCHAR szAppName[] = L"ライブラリ";

//初めのシーン
#define START_SCENE SceneName::Main

//シーン
enum SceneName
{
	Main,
};

enum ObjName
{
	Player,
};
