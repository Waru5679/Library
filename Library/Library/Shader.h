#pragma once
#include <Windows.h>
#include <stdio.h>
#include <d3dx10.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

#include "ObjLoader.h"
#include "Camera.h"
#include "Polygon.h"

//シェーダクラス
class CShader
{
private:
	CShader() {};	//コンストラクタ
	~CShader() {};	//デストラクタ

	//インスタンス
	static CShader* m_pInstance;

public:
	//インスタンス取得
	static CShader* GetInstance()
	{
		//インスタンス化されてなければインスタンス化
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CShader();
		}
		return m_pInstance;
	}
	
	bool Init(ID3D10Device* pDevice);	//初期化
	void Release();						//解放

	//カメラセット
	void SetCamera(CameraBase* pCamera) { m_pCamera = pCamera; }

	//シェーダーセット
	void SetShader(ID3D10ShaderResourceView* pTexture, RECT_F* pSrc, ColorData* pColor, D3DXMATRIX matWorld);

	//アクセサ
	ID3D10Effect*			GetEffect()			{ return m_pEffect; }
	ID3D10EffectTechnique*	GetTechnique()		{ return m_pTechnique; }
	ID3D10InputLayout*		GetVertexLayOut()	{ return m_pVertexLayout; }
private:
	ID3D10Effect* m_pEffect;				//エフェクト
	ID3D10EffectTechnique* m_pTechnique;	//テクニック
	ID3D10InputLayout* m_pVertexLayout;		//頂点レイアウト

	//アプリ←→シェーダー架け橋
	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;	//ワールドから射影までの変換行列
	ID3D10EffectShaderResourceVariable* m_pShaderTexture;		//テクスチャー
	ID3D10EffectVectorVariable* m_pShaderSrc;					//切り取り位置
	ID3D10EffectVectorVariable* m_pShaderColor;					//描画色
	
	CameraBase* m_pCamera;	//カメラポインタ
};

