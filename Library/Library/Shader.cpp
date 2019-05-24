#pragma once
#include "Main.h"
#include "Shader.h"
#include "Camera.h"

CShader g_Shader;

//初期化
HRESULT CShader::Init(ID3D10Device* pDevice)
{
	//エフェクトを作成
	D3DX10CreateEffectFromFile(L"Geometry_Texture.fx", NULL, NULL, "fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
		pDevice, NULL, NULL, &m_pEffect, NULL, NULL);

	//テクニックを得る
	m_pTechnique = m_pEffect->GetTechniqueByName("Render");

	//頂点インプットレイアウトを定義	
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	D3D10_PASS_DESC PassDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &m_pVertexLayout)))
	{
		return E_FAIL;
	}

	//頂点インプットレイアウトをセット
	pDevice->IASetInputLayout(m_pVertexLayout);

	//アプリ←→シェーダー架け橋
	m_pShaderWorldViewProjection = m_pEffect->GetVariableByName("g_mWVP")->AsMatrix();
	m_pShaderTexture = m_pEffect->GetVariableByName("g_texDecal")->AsShaderResource();
	m_pShaderSrc = m_pEffect->GetVariableByName("g_UvSrc")->AsVector();
	m_pShaderColor = m_pEffect->GetVariableByName("g_Color")->AsVector();
	return S_OK;
}

//シェーダーセット
void CShader::SetShader(ID3D10ShaderResourceView* pTexture, float Src[4],float Color[4], D3DXMATRIX matWorld)
{	
	//ワールド＊ビュー*プロジェクション
	D3DXMATRIX objWVP = matWorld *m_pCamera->GetViewMatrix() *m_pCamera->GetProjMatrix();
	m_pShaderWorldViewProjection->SetMatrix((float*)&(objWVP));
	   
	//切り取り位置
	m_pShaderSrc->SetFloatVector(Src);

	//描画色
	m_pShaderColor->SetFloatVector(Color);

	//テクスチャ情報があれば
	if (pTexture != NULL)
	{
		m_pShaderTexture->SetResource(pTexture);
	}
}


//解放
void CShader::Release()
{
	m_pVertexLayout->Release();
	m_pEffect->Release();
}