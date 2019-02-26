#pragma once
#include <Windows.h>
#include <stdio.h>
#include <d3dx10.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

#include "ObjLoader.h"
#include "Camera.h"

class CShader
{
public:
	HRESULT Init();

	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }

	//シェーダーのセット
	void SetShader(D3DXMATRIX matWorld, MY_MATERIAL& material);
private:
	//アプリ←→シェーダー架け橋
	ID3D10EffectMatrixVariable* m_pShaderWorld;//ワールド変換行列
	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;//ワールドから射影までの変換行列
	ID3D10EffectShaderResourceVariable* m_pShaderTexture;//テクスチャー
	ID3D10EffectVectorVariable* m_pShaderAmbient;//アンビエント光
	ID3D10EffectVectorVariable* m_pShaderDiffuse;//拡散反射色
	ID3D10EffectVectorVariable* m_pShaderSpecular;//光沢
	ID3D10EffectVectorVariable* m_pShaderLightDir;//ライト方向
	ID3D10EffectVectorVariable* m_pShaderEyePos;//カメラ位置

	Camera* m_pCamera;

};

extern CShader g_Shader;
