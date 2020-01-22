#pragma once

//初めのシーン
#define START_SCENE SceneName::SceneTitle

//ゲームで使うオブジェクトファイル
#include "../Game/2DPolygon.h"
#include "../Game/Street.h"
#include "../Game/MainCamera.h"
#include "../Game/Enemy.h"
#include "../Game/EnemyBullet.h"
#include "../Game/Bullet.h"
#include "../Game/Player.h"
#include "../Game/Aiming.h"
#include "../Game/TitleCamera.h"
#include "../Game/TitleObj.h"

//Debug
#include "../Game/RayTest.h"
#include "../Game/Sphere.h"

//ゲームで使うシーンファイル
#include "../Game/SceneMain.h"
#include "../Game/SceneTitle.h"

//シーン
enum SceneName
{
	SceneMain,	//メイン
	SceneTitle,	//タイトル
};

//オブジェクトID(処理順)
enum ObjName
{
	ObjPlayer,		//プレイヤー
	ObjMainCamera,	//メインカメラ
	ObjDraw2D,		//2D描画
	ObjSphere,		//球
	ObjEnemy,		//敵
	ObjEnemyBullet,	//敵の弾
	ObjBullet,		//弾
	ObjAiming,		//照準
	ObjTest,		//テスト
	ObjVmdTest,		//vmd
	ObjTitleCamera,	//タイトルカメラ
	ObjTitle,		//タイトルObj
};

//モデル
enum ModelName
{
	ModelPlayer,	//プレイヤー
	ModelSphere,	//球
	ModelAiming,	//照準
	ModelTarget,	//的
};

//テクスチャ
enum TexName
{
	TexTest	,	//描画テスト
};

//定数
constexpr int BULLET_DELETE_TIME{ 300 };	//弾の削除タイム