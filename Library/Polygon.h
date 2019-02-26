#pragma once

#include "Main.h"
#include "ObjLoader.h"
#include <windef.h>

struct RECT_F
{
	float m_top;
	float m_left;
	float m_right;
	float m_bottom;
};

//描画クラス
typedef class CDrawPolygon
{
public:
	//初期化
	void Init();

	//描画
	void Draw3D(int TexId, D3DXMATRIX matWorld);
	void Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld);
	void Draw2D(int TexId,RECT_F* Out);
	void Draw2D(ID3D10ShaderResourceView* pResView,RECT_F*Out);

	//テクスチャ読み込み
	void LoadTexture(int Id, const char* Name);
	
private:
	void DrawPolygon();		//ポリゴン描画
	HRESULT CreateBartex();	//バーテックスバッファの作成


	MY_MATERIAL m_material;	//マテリアル
	MY_VERTEX* m_vertex;	//頂点
	ID3D10Buffer* m_pBuffer;//バーテックスバッファ
	
	D3DXVECTOR3 m_vPos;		//位置
	D3DXVECTOR3 m_vAngle;	//回転
	D3DXVECTOR3 m_vScale;	//大きさ

	D3DXMATRIX m_matWorld;	//ワールド行列

}CDraw;

extern CDraw g_Draw;