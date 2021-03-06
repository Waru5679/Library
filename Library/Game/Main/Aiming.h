#pragma once
#include "../../Library/CObj.h"
#include "Player.h"
#include "MainCamera.h"
//照準クラス
class CAiming :public CObj3DBase
{
public:
	//コンストラクタ
	CAiming(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	void Init();	//描画
	void Update();	//更新
	void Draw();	//描画
	void Release();	//開放
private:
	D3DXVECTOR3 m_vMove;	//移動ベクトル
	CPlayer* m_pPlayer;		//プレイヤーのポインタ
	CMainCamera* m_pCamera;	//カメラポインタ
};