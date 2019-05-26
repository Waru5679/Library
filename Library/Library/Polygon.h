#pragma once

#include "Main.h"
#include "Camera.h"
#include "ObjLoader.h"
#include "Rect.h"
#include <windef.h>

//描画クラス
class CDraw
{
public:	
	void Init();	//初期化
	void Release();	//解放

	//描画
	void Draw3D(int TexId, D3DXMATRIX matWorld);
	void Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld);

	//2D描画(Fontなど切り取り位置のないもの用の中継)
	void Draw2D(int TexId, RECT_F* pSrc, RECT_F* pOut, float fColor[4], float fRad);

	//2D描画(テクスチャのサイズから切り取り位置の設定がいるもの用中継)
	void Draw2D(ID3D10ShaderResourceView* pTex, RECT_F* pOut, float fRad);

	//2D描画
	void Draw2D(ID3D10ShaderResourceView* pTex, float fSrc[4], RECT_F* pOut, float fColor[4], float fRad);
	
	//テクスチャ読み込み
	void LoadTexture(int Id, const char* Name,int Width, int Height);

	//カメラセット
	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }	
private:
	void DrawPolygon();		//ポリゴン描画
	HRESULT CreateBartex();	//バーテックスバッファの作成
	
	ID3D10Buffer* m_pBuffer;//バーテックスバッファ
	MY_VERTEX* m_Vertex;	//頂点
	   	
	Camera* m_pCamera;	//カメラポインタ
};

extern CDraw g_Draw;