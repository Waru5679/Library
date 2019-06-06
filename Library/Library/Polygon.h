#pragma once

#include "Main.h"
#include "Camera.h"
#include "Struct.h"

//描画クラス
class CDraw
{
public:	
	void Init();	//初期化
	void Release();	//解放

	//バッファ作成
	ID3D10Buffer* BufferCreate(void* pHead, unsigned int Size, int BufferType);

	//2D描画(テクスチャのサイズから切り取り位置の設定がいるもの用中継)
	void DrawTexture(int TexId, RECT_F* pSrc, RECT_F* pOut, ColorData* pColor, float fRad);

	//2D描画
	void DrawTexture(ID3D10ShaderResourceView* pTex, RECT_F* pSrc, RECT_F* pOut, ColorData* pColor, float fRad);
	
	//テクスチャ読み込み
	void LoadTexture(int Id, const char* Name,int Width, int Height);

	//カメラセット
	void SetCamera(CameraBase* pCamera) { m_pCamera = pCamera; }

	//ポリゴン描画
	void DrawPolygon(int VerNum, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer);
private:
	
	ID3D10Buffer* m_pVertexBuffer;//バーテックスバッファ
	  	
	CameraBase* m_pCamera;	//カメラポインタ
};

extern CDraw g_Draw;