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

//初めのシーン
#define START_SCENE SceneName::SceneMain 

//コールバック関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

#define DX CDirectX::GetInstance()
#define SHADER CShader::GetInstance()
#define FRAME CFrameRate::GetInstance()
#define SCENE CSceneManager::GetInstance()
#define AUDIO CAudio::GetInstance()
#define FONT CFont::GetInstance()
#define DRAW CDraw::GetInstance()
#define HIT CHit::GetInstance()

//
//#include "Release.h"
//#define RELEASE CRelease::GetInstance()

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
