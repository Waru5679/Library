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

	//バーテックスバッファ作成
	ID3D10Buffer* BufferCreate(void* pHead, unsigned int Size, int BufferType);

	//2D描画(Fontなど切り取り位置のないもの用の中継)
	void DrawTexture(int TexId, RECT_F* pSrc, RECT_F* pOut, float fColor[4], float fRad);

	//2D描画(テクスチャのサイズから切り取り位置の設定がいるもの用中継)
	void DrawTexture(ID3D10ShaderResourceView* pTex, RECT_F* pOut, float fRad);

	//2D描画
	void DrawTexture(ID3D10ShaderResourceView* pTex, float fSrc[4], RECT_F* pOut, float fColor[4], float fRad);
	
	//テクスチャ読み込み
	void LoadTexture(int Id, const char* Name,int Width, int Height);

	//カメラセット
	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }	

	//ポリゴン描画
	void DrawPolygon(int VerNum, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer);

private:
	
	ID3D10Buffer* m_pVertexBuffer;//バーテックスバッファ
	  	
	Camera* m_pCamera;	//カメラポインタ
};

extern CDraw g_Draw;