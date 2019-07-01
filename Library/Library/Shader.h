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
private:
	CShader() {};	//�R���X�g���N�^
	~CShader() {};	//�f�X�g���N�^

	//�C���X�^���X
	static CShader* m_pInstance;

public:
	//�C���X�^���X�擾
	static CShader* GetInstance()
	{
		//�C���X�^���X������ĂȂ���΃C���X�^���X��
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CShader();
		}
		return m_pInstance;
	}
	
	bool Init(ID3D10Device* pDevice);	//������
	void Release();						//���

	//�J�����Z�b�g
	void SetCamera(CameraBase* pCamera) { m_pCamera = pCamera; }

	//�V�F�[�_�[�Z�b�g
	void SetShader(ID3D10ShaderResourceView* pTexture, RECT_F* pSrc, ColorData* pColor, D3DXMATRIX matWorld);

	//�A�N�Z�T
	ID3D10Effect*			GetEffect()			{ return m_pEffect; }
	ID3D10EffectTechnique*	GetTechnique()		{ return m_pTechnique; }
	ID3D10InputLayout*		GetVertexLayOut()	{ return m_pVertexLayout; }
private:
	ID3D10Effect* m_pEffect;				//�G�t�F�N�g
	ID3D10EffectTechnique* m_pTechnique;	//�e�N�j�b�N
	ID3D10InputLayout* m_pVertexLayout;		//���_���C�A�E�g

	//�A�v�������V�F�[�_�[�˂���
	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;	//���[���h����ˉe�܂ł̕ϊ��s��
	ID3D10EffectShaderResourceVariable* m_pShaderTexture;		//�e�N�X�`���[
	ID3D10EffectVectorVariable* m_pShaderSrc;					//�؂���ʒu
	ID3D10EffectVectorVariable* m_pShaderColor;					//�`��F
	
	CameraBase* m_pCamera;	//�J�����|�C���^
};

