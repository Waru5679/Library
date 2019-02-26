#pragma once
#include "Main.h"
#include "ObjLoader.h"


//オブジェクト　継承用クラス
class CObj
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	int m_id;
	bool m_fDelete;

protected:
	//ベクトル
	D3DXVECTOR3 m_vPos;		//位置
	D3DXVECTOR3 m_vAngle;	//回転
	D3DXVECTOR3 m_vScale;	//大きさ

	//行列
	D3DXMATRIX m_matWorld;	//ワールド
	D3DXMATRIX m_matTrans;	//移動
	D3DXMATRIX m_matRot;	//回転
	D3DXMATRIX m_matScale;	//拡大・縮小
};

//2Dオブジェクト　継承用クラス
class CObj2D:public CObj
{
public:
protected:
	D3DXVECTOR2 m_ScreenPos;	//描画位置(左上)
};

//3Dモデルを使用するもの　継承用クラス
class CObj3D :public CObj
{
public:
protected:
	MY_MESH m_Mesh;	//モデル

	D3DXVECTOR3 m_vMin;	//最小
	D3DXVECTOR3 m_vMax; //最大
};

