#pragma once
#include "Main.h"

//DirectX�N���X
class CDirectX
{
public:

	//������
	HRESULT Init(HWND hWnd);

	//�������̊J��
	void Release();

	ID3D10Device*			m_pDevice;				//�f�o�C�X
	IDXGISwapChain*			m_pSwapChain;			//�X���b�v�`�F�C��
	ID3D10RenderTargetView* m_pRenderTargetView;	//�����_�[�^�[�Q�b�g�r���[
	ID3D10DepthStencilView* m_pDepthStencilView;	//�[�x�X�e���V���r���[
	ID3D10Texture2D*		m_pDepthStencil;		//�[�x�X�e���V��
	ID3D10BlendState*		m_pBlendState;			//�u�����h�X�e�[�g
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
};

extern CDirectX dx;