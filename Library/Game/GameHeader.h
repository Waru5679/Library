#pragma once

//初めのシーン
#define START_SCENE SceneName::SceneMain 

//ゲームで使うオブジェクトファイル
#include "../Game/2DPolygon.h"
#include "../Game/Street.h"
#include "../Game/MainCamera.h"
#include "../Game/Enemy.h"
#include "../Game/EnemyBullet.h"
#include "../Game/Bullet.h"
#include "../Game/Player.h"
#include "../Game/Aiming.h"

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
	ObjPlayer,		//プレイヤー
	ObjDraw2D,		//2D描画
	ObjSphere,		//球
	ObjEnemy,		//敵
	ObjEnemyBullet,	//敵の弾
	ObjBullet,		//弾
	ObjAiming,		//照準
};

//モデル
enum ModelName
{
	ModelPlayer,	//プレイヤー
	ModelSphere,	//球
	ModelAiming,	//照準
};

//テクスチャ
enum TexName
{
	TexTest	,	//描画テスト
};