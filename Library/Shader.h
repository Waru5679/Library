#pragma once
#include <Windows.h>
#include <stdio.h>
#include <d3dx10.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

#include "ObjLoader.h"
#include "Camera.h"

//シェーダクラス
class CShader
{
public:
	//初期化
	HRESULT Init(ID3D10Device* pDevice);

	//カメラセット
	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }

	//シェーダーセット
	void SetShader(ID3D10ShaderResourceView* pTexture,D3DXMATRIX matWorld);

	//解放
	void Release();

	ID3D10Effect*           m_pEffect;			//エフェクト
	ID3D10EffectTechnique*  m_pTechnique;		//テクニック
	ID3D10InputLayout*      m_pVertexLayout;	//頂点レイアウト
private:
	//アプリ←→シェーダー架け橋
	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;	//ワールドから射影までの変換行列
	ID3D10EffectShaderResourceVariable* m_pShaderTexture;		//テクスチャー
	
	Camera* m_pCamera;	//カメラポインタ
};

extern CShader g_Shader;
