#pragma once
#include "Main.h"
#include "Shader.h"
#include "Camera.h"

CShader g_Shader;

//ライトの位置
D3DXVECTOR3 g_vLightVector(1,-1,1);

HRESULT CShader::Init(ID3D10Device* pDevice)
{
	//エフェクトを作成
	D3DX10CreateEffectFromFile(L"Geometry_Material_Texture.fx", NULL, NULL, "fx_4_0",
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

	return S_OK;
}


void CShader::SetShader(D3DXMATRIX matWorld,MY_MATERIAL& material)
{	
	// 変換行列をエフェクト（シェーダー）に渡す
	m_pShaderWorld = m_pEffect->GetVariableByName("g_mW")->AsMatrix();
	m_pShaderWorld->SetMatrix((float*)&matWorld);
	m_pShaderWorldViewProjection = m_pEffect->GetVariableByName("g_mWVP")->AsMatrix();

	D3DXMATRIX objWVP = matWorld *m_pCamera->GetViewMatrix() *m_pCamera->GetProjMatrix();
	m_pShaderWorldViewProjection->SetMatrix((float*)&(objWVP));
	
	//マテリアルの各要素をエフェクト（シェーダー）に渡す
	m_pShaderAmbient = m_pEffect->GetVariableByName("g_Ambient")->AsVector();
	m_pShaderDiffuse = m_pEffect->GetVariableByName("g_Diffuse")->AsVector();
	m_pShaderSpecular = m_pEffect->GetVariableByName("g_Specular")->AsVector();
	m_pShaderAmbient->SetFloatVector((float*)&material.Ka);
	m_pShaderDiffuse->SetFloatVector((float*)&material.Kd);
	m_pShaderSpecular->SetFloatVector((float*)&material.Ks);
	
	//テクスチャ情報があれば
	if (material.pTexture != NULL)
	{
		m_pShaderTexture = m_pEffect->GetVariableByName("g_texDecal")->AsShaderResource();
		m_pShaderTexture->SetResource(material.pTexture);
	}

	//ライトベクトルをエフェクト（シェーダー）に渡す
	m_pShaderLightDir = m_pEffect->GetVariableByName("g_vLight")->AsVector();
	m_pShaderLightDir->SetFloatVector((float*)&g_vLightVector);

	//視線ベクトルをエフェクト（シェーダー）に渡す
	m_pShaderEyePos = m_pEffect->GetVariableByName("g_vEye")->AsVector();
	m_pShaderEyePos->SetFloatVector((float*)&m_pCamera->GetEye());
}


//解放
void CShader::Release()
{
	m_pVertexLayout->Release();
	m_pEffect->Release();
}