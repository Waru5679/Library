#pragma once

//ライブラリファイル
#include "../Library/Main.h"
#include "../Library/Camera.h"

//ゲームファイル
#include "Player.h"

class CMainCamera :public Camera
{
public:
	void Init();	//初期化
	void Update();	//更新
	void Draw() {};	//描画

private:
	D3DXVECTOR3 m_vPlayerPos;	//プレイヤーの位置
	D3DXVECTOR3 m_vPlayerFront;	//プレイヤーの正面方向
	
	float m_fCameraDis;	//プレイヤーとカメラの距離
	float m_fLookDis;	//プレイヤーと注視点の距離

	D3DXMATRIX m_matRot;	//回転行列

};