#pragma once

//必要なヘッダーファイルのインクルード
#include <windows.h>
#include <D3D10.h>
#include <d3dx10.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

//ウインドウサイズ
constexpr int WINDOW_WIDTH{ 640 };
constexpr int WINDOW_HEIGHT{ 480 };

//フレームレート
constexpr int FRAME_RATE{ 60 };

//DirectX
#define DX CDirectX::GetInstance()

////window初期設定
//void WinInit(HINSTANCE hInst);

////ウインドウ名
//static WCHAR szAppName[] = L"ライブラリ";

//コールバック関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

//初めのシーン
#define START_SCENE SceneName::SceneMain 

//シーン
enum SceneName
{
	SceneMain,	//メイン
};

//オブジェクト
enum ObjName
{
	ObjMainCamera,	//メインカメラ
	ObjPlayer,		//プレイヤー
	ObjDraw2D,		//2D描画
	ObjSphere,		//球
	ObjEnemy,		//敵
	ObjBullet,		//弾
};

//モデル
enum ModelName
{
	ModelPlayer,	//プレイヤー
	ModelSphere,	//球
};
