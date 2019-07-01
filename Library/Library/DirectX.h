#pragma once
#include "Main.h"

//DirectX�N���X
class CDirectX
{
private:
	CDirectX() {};	//�R���X�g���N�^
	~CDirectX() {};	//�f�X�g���N�^

	//�C���X�^���X
	static CDirectX* m_pInstance;
public:
	//�C���X�^���X�擾
	static CDirectX* GetInstance()
	{
		//�C���X�^���X������ĂȂ���΃C���X�^���X��
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CDirectX();
		}

		return m_pInstance;
	}

	//������
	bool Init(HWND hWnd);

	//�������̊J��
	void Release();

	//�A�N�Z�T
	ID3D10Device*			GetDevice()				{ return m_pDevice; }	
	IDXGISwapChain*			GetSwapChain()			{ return m_pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView()	{ return m_pRenderTargetView; }
	ID3D10DepthStencilView* GetDepthStencilView()	{ return m_pDepthStencilView; }
	ID3D10Texture2D*		GetDepthStencil()		{ return m_pDepthStencil; }
	ID3D10BlendState*		GetBlendState()			{ return m_pBlendState; }
private:
	//�X���b�v�`�F�C���쐬
	HRESULT CreateSwapChain(HWND hWnd);

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	void CreateRenderTarget();

	//�r���[�|�[�g�ݒ�
	void SetViewport();

	//�[�x�X�e���V���r���[�̍쐬
	void CreateStencil();

	//�u�����h�X�e�[�g�̍쐬
	void CreateBlendState();

	//���X�^���C�U�ݒ�
	void SetRasterizer();	

	ID3D10Device* m_pDevice;					//�f�o�C�X
	IDXGISwapChain* m_pSwapChain;				//�X���b�v�`�F�C��
	ID3D10RenderTargetView* m_pRenderTargetView;//�����_�[�^�[�Q�b�g�r���[
	ID3D10DepthStencilView* m_pDepthStencilView;//�[�x�X�e���V���r���[
	ID3D10Texture2D* m_pDepthStencil;			//�[�x�X�e���V��
	ID3D10BlendState* m_pBlendState;			//�u�����h�X�e�[�g
};
