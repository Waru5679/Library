#pragma once
#include "Main.h"

class CDirectX
{
public:

	CDirectX() {};
	~CDirectX() {};
	
	//初期化
	HRESULT Init(HWND hWnd);

	//メモリの開放
	void Release();

	ID3D10Device*			m_pDevice;
	IDXGISwapChain*			m_pSwapChain;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;
	ID3D10Effect*           m_pEffect;
	ID3D10EffectTechnique*  m_pTechnique;
	ID3D10Texture2D*		m_pDepthStencil;
	ID3D10InputLayout*      m_pVertexLayout;

	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;//アプリ←→シェーダー架け橋　ワールドから射影までの変換行列
private:
	//スワップチェイン作成
	HRESULT CreateSwapChain(HWND hWnd);

	//レンダーターゲットビューの作成
	void CreateRenderTarget();

	//ビューポート設定
	void SetViewport();

	//深度ステンシルビューの作成
	void CreateStencil();

	//ラスタライザの作成
	void CreateRasterizer();	
};

extern CDirectX dx;