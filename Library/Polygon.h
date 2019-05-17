#pragma once

#include "Main.h"
#include "Camera.h"
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
class CDraw
{
public:
	//初期化
	void Init();

	//描画
	void Draw3D(int TexId, D3DXMATRIX matWorld);
	void Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld);
	void Draw2D(int TexId, RECT_F* pSrc, RECT_F* pOut, float fColor[4], float fRad);
	void Draw2D(ID3D10ShaderResourceView* pTex, RECT_F* pOut, float fRad);
	//テクスチャ読み込み
	void LoadTexture(int Id, const char* Name,int Width, int Height);

	//カメラセット
	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }

	//解放
	void Release();
	
private:
	void DrawPolygon();		//ポリゴン描画
	HRESULT CreateBartex();	//バーテックスバッファの作成
	
	ID3D10Buffer* m_pBuffer;//バーテックスバッファ
	MY_VERTEX* m_Vertex;	//頂点
	   	
	Camera* m_pCamera;	//カメラポインタ
};

//RECT_Fのセット
void RectSet(float x, float y, float Size_x, float Size_y,RECT_F* pOut );


extern CDraw g_Draw;