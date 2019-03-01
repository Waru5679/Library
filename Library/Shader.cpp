#pragma once
#include "Main.h"
#include "Shader.h"
#include "Camera.h"

CShader g_Shader;

//���C�g�̈ʒu
D3DXVECTOR3 g_vLightVector(1,-1,1);

HRESULT CShader::Init(ID3D10Device* pDevice)
{
	//�G�t�F�N�g���쐬
	D3DX10CreateEffectFromFile(L"Geometry_Material_Texture.fx", NULL, NULL, "fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
		pDevice, NULL, NULL, &m_pEffect, NULL, NULL);

	//�e�N�j�b�N�𓾂�
	m_pTechnique = m_pEffect->GetTechniqueByName("Render");

	//���_�C���v�b�g���C�A�E�g���`	
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	D3D10_PASS_DESC PassDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &m_pVertexLayout)))
	{
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDevice->IASetInputLayout(m_pVertexLayout);

	return S_OK;
}


void CShader::SetShader(D3DXMATRIX matWorld,MY_MATERIAL& material)
{	
	// �ϊ��s����G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
	m_pShaderWorld = m_pEffect->GetVariableByName("g_mW")->AsMatrix();
	m_pShaderWorld->SetMatrix((float*)&matWorld);
	m_pShaderWorldViewProjection = m_pEffect->GetVariableByName("g_mWVP")->AsMatrix();

	D3DXMATRIX objWVP = matWorld *m_pCamera->GetViewMatrix() *m_pCamera->GetProjMatrix();
	m_pShaderWorldViewProjection->SetMatrix((float*)&(objWVP));
	
	//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
	m_pShaderAmbient = m_pEffect->GetVariableByName("g_Ambient")->AsVector();
	m_pShaderDiffuse = m_pEffect->GetVariableByName("g_Diffuse")->AsVector();
	m_pShaderSpecular = m_pEffect->GetVariableByName("g_Specular")->AsVector();
	m_pShaderAmbient->SetFloatVector((float*)&material.Ka);
	m_pShaderDiffuse->SetFloatVector((float*)&material.Kd);
	m_pShaderSpecular->SetFloatVector((float*)&material.Ks);
	
	//�e�N�X�`����񂪂����
	if (material.pTexture != NULL)
	{
		m_pShaderTexture = m_pEffect->GetVariableByName("g_texDecal")->AsShaderResource();
		m_pShaderTexture->SetResource(material.pTexture);
	}

	//���C�g�x�N�g�����G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
	m_pShaderLightDir = m_pEffect->GetVariableByName("g_vLight")->AsVector();
	m_pShaderLightDir->SetFloatVector((float*)&g_vLightVector);

	//�����x�N�g�����G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
	m_pShaderEyePos = m_pEffect->GetVariableByName("g_vEye")->AsVector();
	m_pShaderEyePos->SetFloatVector((float*)&m_pCamera->GetEye());
}


//���
void CShader::Release()
{
	m_pVertexLayout->Release();
	m_pEffect->Release();
}