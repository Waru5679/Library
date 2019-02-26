#include "DirectX.h"
#include "CObj.h"
#include "Camera.h"
#include "Shader.h"
#include "ObjLoader.h"

CDirectX dx;

HRESULT CDirectX::Init(HWND hWnd)
{
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	if (FAILED(CreateSwapChain(hWnd)))
	{
		return FALSE;
	}	

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	CreateRenderTarget();

	//�r���[�|�[�g�̐ݒ�
	SetViewport();

	//�[�x�X�e���V���r���[�̍쐬
	CreateStencil();		

	//���X�^���C�U�ݒ�
	CreateRasterizer();

	//�V�F�[�_�[������
	if (FAILED(g_Shader.Init()))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

//�X���b�v�`�F�C���쐬
HRESULT CDirectX::CreateSwapChain(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL,
		0, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice)))
	{
		return FALSE;
	}

	return S_OK;
}

//�r���[�|�[�g�ݒ�
void CDirectX::SetViewport()
{
	D3D10_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	dx.m_pDevice->RSSetViewports(1, &vp);
}

//�����_�[�^�[�Q�b�g�r���[�̍쐬
void CDirectX::CreateRenderTarget()
{
	ID3D10Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
}

//�[�x�X�e���V���r���[�̍쐬
void CDirectX::CreateStencil()
{
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	dx.m_pDevice->CreateTexture2D(&descDepth, NULL, &dx.m_pDepthStencil);

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	dx.m_pDevice->CreateDepthStencilView(dx.m_pDepthStencil, &descDSV, &dx.m_pDepthStencilView);

	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	dx.m_pDevice->OMSetRenderTargets(1, &dx.m_pRenderTargetView, dx.m_pDepthStencilView);
}

//���X�^���C�U�̍쐬
void CDirectX::CreateRasterizer()
{
	//���X�^���C�Y�ݒ�
	D3D10_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D10_CULL_NONE;//CCW
	rdc.FillMode = D3D10_FILL_SOLID;

	ID3D10RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDevice->RSSetState(pIr);
}

//�������J��
void CDirectX::Release()
{
	m_pDepthStencil->Release();
	m_pDepthStencilView->Release();
	m_pEffect->Release();
	m_pSwapChain->Release();
	m_pRenderTargetView->Release();
	m_pVertexLayout->Release();
	m_pDevice->Release();
}