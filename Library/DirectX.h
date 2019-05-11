#pragma once
#include "Main.h"

//DirectXクラス
class CDirectX
{
public:

	//初期化
	HRESULT Init(HWND hWnd);

	//メモリの開放
	void Release();

	ID3D10Device*			m_pDevice;				//デバイス
	IDXGISwapChain*			m_pSwapChain;			//スワップチェイン
	ID3D10RenderTargetView* m_pRenderTargetView;	//レンダーターゲットビュー
	ID3D10DepthStencilView* m_pDepthStencilView;	//深度ステンシルビュー
	ID3D10Texture2D*		m_pDepthStencil;		//深度ステンシル
	ID3D10BlendState*		m_pBlendState;			//ブレンドステート
private:
	//スワップチェイン作成
	HRESULT CreateSwapChain(HWND hWnd);

	//レンダーターゲットビューの作成
	void CreateRenderTarget();

	//ビューポート設定
	void SetViewport();

	//深度ステンシルビューの作成
	void CreateStencil();

	//ブレンドステートの作成
	void CreateBlendState();

	//ラスタライザ設定
	void SetRasterizer();	
};

extern CDirectX dx;