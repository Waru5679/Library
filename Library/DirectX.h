#pragma once
#include "Main.h"

class CDirectX
{
public:

	CDirectX() {};
	~CDirectX() {};
	
	//������
	HRESULT Init(HWND hWnd);

	//�������̊J��
	void Release();

	ID3D10Device*			m_pDevice;
	IDXGISwapChain*			m_pSwapChain;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;
	ID3D10Effect*           m_pEffect;
	ID3D10EffectTechnique*  m_pTechnique;
	ID3D10Texture2D*		m_pDepthStencil;
	ID3D10InputLayout*      m_pVertexLayout;

	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;//�A�v�������V�F�[�_�[�˂����@���[���h����ˉe�܂ł̕ϊ��s��
private:
	//�X���b�v�`�F�C���쐬
	HRESULT CreateSwapChain(HWND hWnd);

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	void CreateRenderTarget();

	//�r���[�|�[�g�ݒ�
	void SetViewport();

	//�[�x�X�e���V���r���[�̍쐬
	void CreateStencil();

	//���X�^���C�U�̍쐬
	void CreateRasterizer();	
};

extern CDirectX dx;