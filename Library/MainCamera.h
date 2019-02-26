#pragma once

#include "Main.h"
#include "Player.h"
#include "Camera.h"

class CMainCamera :public Camera
{
public:
	void Init();	//初期化
	void Update();	//更新
	
private:
	
	D3DXVECTOR3 m_vPlayerPos;		//プレイヤーの位置
	D3DXVECTOR3 m_vPlayerFront;			//プレイヤーの正面方向
	
	float m_fCameraDis;		//プレイヤーとカメラの距離
	float m_fLookDis;		//プレイヤーと注視点の距離

	D3DXMATRIX m_matRot;	//回転行列

};