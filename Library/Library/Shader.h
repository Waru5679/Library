#pragma once
#include <Windows.h>
#include <stdio.h>
#include <d3dx10.h>

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

#include "ObjLoader.h"
#include "Camera.h"
#include "Polygon.h"

//�V�F�[�_�N���X
class CShader
{
public:
	//������
	bool Init(ID3D10Device* pDevice);

	//�J�����Z�b�g
	void SetCamera(CameraBase* pCamera) { m_pCamera = pCamera; }

	//�V�F�[�_�[�Z�b�g
	void SetShader(ID3D10ShaderResourceView* pTexture, RECT_F* pSrc, ColorData* pColor, D3DXMATRIX matWorld);

	//���
	void Release();

	ID3D10Effect*           m_pEffect;			//�G�t�F�N�g
	ID3D10EffectTechnique*  m_pTechnique;		//�e�N�j�b�N
	ID3D10InputLayout*      m_pVertexLayout;	//���_���C�A�E�g
private:
	//�A�v�������V�F�[�_�[�˂���
	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;	//���[���h����ˉe�܂ł̕ϊ��s��
	ID3D10EffectShaderResourceVariable* m_pShaderTexture;		//�e�N�X�`���[
	ID3D10EffectVectorVariable* m_pShaderSrc;					//�؂���ʒu
	ID3D10EffectVectorVariable* m_pShaderColor;					//�`��F
	
	CameraBase* m_pCamera;	//�J�����|�C���^
};

extern CShader g_Shader;
