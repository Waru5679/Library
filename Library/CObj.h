#pragma once
#include "Main.h"
#include "ObjLoader.h"

//オブジェクト　継承用クラス(ゲームオブジェクト全てに継承させる)
class CObj
{
public:
	virtual void Init() = 0;	//初期化
	virtual void Update() = 0;	//更新
	virtual void Draw() = 0;	//描画

	int m_id;		//オブジェクトID
	bool m_fDelete;	//削除フラグ

protected:
};

//2Dオブジェクト　継承用クラス
class CObj2D:public CObj
{
protected:
	D3DXVECTOR2 m_ScreenPos;	//描画位置(左上)
	D3DXVECTOR2 m_Move;			//移動ベクトル

};

//3Dモデルを使用するもの　継承用クラス
class CObj3D :public CObj
{
public :
	MY_MESH* GetMesh() { return m_pMesh; }
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

	MY_MESH* m_pMesh;	//モデルのポインタ
};

