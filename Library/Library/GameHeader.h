#pragma once

//初めのシーン
#define START_SCENE SceneName::SceneMain 

//ゲームで使うファイル
#include "../Game/2DPolygon.h"
#include "../Game/Street.h"
#include "../Game/MainCamera.h"
#include "../Game/Enemy.h"
#include "../Game/Bullet.h"
#include "../Game/Player.h"

//Debug
#include "../Game/RayTest.h"
#include "../Game/Sphere.h"

//ゲームで使うシーンファイル
#include "../Game/SceneMain.h"

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