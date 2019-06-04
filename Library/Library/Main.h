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
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//メモリの開放
void Release();

//ウインドウ名
static WCHAR szAppName[] = L"ライブラリ";

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
	ObjStreet,		//町
	ObjRayTest,	//Rayのテスト
	ObjSphere,	//球
};

//モデル
enum ModelName
{
	ModelStreet,	//町
	ModelPlayer,	//プレイヤー

	ModelRayTest,	//Rayテスト
	ModelSphere,	//球
};
