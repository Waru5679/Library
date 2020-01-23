#pragma once

//初めのシーン
#define START_SCENE SceneName::SceneMain

//ゲームで使うオブジェクトファイル
#include "../Game/Main/2DPolygon.h"
#include "../Game/Main/MainCamera.h"
#include "../Game/Main/Enemy.h"
#include "../Game/Main/EnemyBullet.h"
#include "../Game/Main/Bullet.h"
#include "../Game/Main/Player.h"
#include "../Game/Main/Aiming.h"
#include "../Game/Title/TitleCamera.h"
#include "../Game/Title/TitleObj.h"

//ゲームで使うシーンファイル
#include "../Game/Main/SceneMain.h"
#include "../Game/Title/SceneTitle.h"

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
	TexTitle,	//タイトルテクスチャ
};

//定数
constexpr int BULLET_DELETE_TIME{ 300 };	//弾の削除タイム