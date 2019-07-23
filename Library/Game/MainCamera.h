#pragma once

//ライブラリファイル
#include "../Library/Camera.h"

//ゲームファイル
#include "Player.h"

//メインカメラクラス
class CMainCamera :public CCameraBase
{
public:
	void Init();	//初期化
	void Update();	//更新
	void Draw() {};	//描画
	void Release();	//開放

private:
	D3DXVECTOR3 m_vPlayerPos;	//プレイヤーの位置
	D3DXVECTOR3 m_vPlayerFront;	//プレイヤーの正面方向
	
	float m_fCameraDis;	//プレイヤーとカメラの距離
	float m_fLookDis;	//プレイヤーと注視点の距離

	D3DXMATRIX m_matRot;	//回転行列
};