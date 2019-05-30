#include "Polygon.h"
#include "DirectX.h"
#include "Task.h"
#include "Shader.h"
#include "Camera.h"
#include "Math.h"
#include "Main.h"

CDraw g_Draw;

//������
void CDraw::Init()
{
	//���_���
	MY_VERTEX ver[]=
	{
		D3DXVECTOR3(-1.0f,  1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),	//����
		D3DXVECTOR3( 1.0f,  1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),	//�E��
		D3DXVECTOR3(-1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),	//����
		D3DXVECTOR3( 1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f)	//�E��
	};
	m_Vertex = ver;

	//�o�[�e�b�N�X�o�b�t�@�쐬
	CreateBartex();
}

//�`�� ���p
void CDraw::Draw3D(int TexId, D3DXMATRIX matWorld) 
{	
	Draw3D(g_Task.GetTex(TexId)->m_pTex, matWorld);
}

//�`��
void CDraw::Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld)
{
	//�`��F
	float Color[4] = { 1.0f,1.0f,1.0f,1.0f };

	//�e�N�X�`���؂���ʒu
	float Src[4] = { 0.0f,0.0f,1.0f,1.0f };

	//�V�F�[�_�[�̃Z�b�g
	g_Shader.SetShader(pResView,Src,Color,matWorld);

	//�|���S���̕`��
	DrawPolygon();
}

//2D�`��(Font�Ȃǐ؂���ʒu�̂Ȃ����̗p�̒��p)
void CDraw::Draw2D(ID3D10ShaderResourceView* pTex, RECT_F* pOut, float fRad)
{
	//�؂���i���{�j
	float fSrc[4] = {0.0f,0.0f,1.0f,1.0f};

	//�`��F
	float fColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	
	Draw2D(pTex, fSrc, pOut, fColor, fRad);
}

//2D�`��(�e�N�X�`���̃T�C�Y����؂���ʒu�̐ݒ肪������̗p���p)
void CDraw::Draw2D(int TexId, RECT_F* pSrc,RECT_F* pOut,float fColor[4],float fRad)
{
	//�e�N�X�`�����
	MY_TEXTURE* pTex=nullptr;
	pTex=g_Task.GetTex(TexId);

	//�؂���ʒu�̐ݒ�
	float fSrc[4];
	fSrc[0] = pSrc->m_left	/ pTex->m_Width;
	fSrc[1] = pSrc->m_top	/ pTex->m_Height;
	fSrc[2] = pSrc->m_right	/ pTex->m_Width;
	fSrc[3] = pSrc->m_bottom/ pTex->m_Height;

	Draw2D(pTex->m_pTex, fSrc, pOut, fColor, fRad);
}

//2D�`��
void CDraw::Draw2D(ID3D10ShaderResourceView* pTex, float fSrc[4], RECT_F* pOut, float Color[4], float fRad)
{
	//�t�r���[�s��
	D3DXMATRIX matInvView;
	D3DXMatrixIdentity(&matInvView);
	D3DXMatrixInverse(&matInvView, NULL, &m_pCamera->GetViewMatrix());

	//�t�v���W�F�N�V�����s��
	D3DXMATRIX matInvProj;
	D3DXMatrixIdentity(&matInvProj);
	D3DXMatrixInverse(&matInvProj, NULL, &m_pCamera->GetProjMatrix());

	//���[���h�s��
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	//�T�C�Y�����߂�
	D3DXVECTOR2 OutSize;
	OutSize.x = pOut->m_right - pOut->m_left;
	OutSize.y = pOut->m_bottom - pOut->m_top;
	matWorld._11 = OutSize.x / WINDOW_WIDTH;
	matWorld._22 = OutSize.y / WINDOW_HEIGHT;

	//Z����]�s��
	D3DXMATRIX matRot;
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(fRad));

	//���[���h�ɉ�]��������
	matWorld *= matRot;

	//1�s�N�Z���̑傫�������߂�
	D3DXVECTOR2 PixcelSize;
	PixcelSize.x = (2.0f / (float)WINDOW_WIDTH);
	PixcelSize.y = (2.0f / (float)WINDOW_HEIGHT);

	//�}�C�i�X����ɗ���悤�ɏ㉺���]����
	pOut->m_top = WINDOW_HEIGHT - pOut->m_bottom;
	pOut->m_bottom = WINDOW_HEIGHT - pOut->m_top;

	//���s�ړ��ʂ����߂�
	matWorld._41 = PixcelSize.x * pOut->m_left + PixcelSize.x * (OutSize.x / 2.0f) - 1.0f;
	matWorld._42 = PixcelSize.y * pOut->m_top + PixcelSize.y * (OutSize.y / 2.0f) - 1.0f;

	//�V�F�[�_�[�̃Z�b�g
	g_Shader.SetShader(pTex, fSrc, Color, matWorld *matInvProj*matInvView);

	//�|���S���̕`��
	DrawPolygon();
}

//�o�[�e�b�N�X�o�b�t�@�[�쐬
HRESULT CDraw::CreateBartex()
{
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_VERTEX) * 4;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_Vertex;
	if (FAILED(dx.m_pDevice->CreateBuffer(&bd, &InitData, &m_pBuffer)))
		return FALSE;

	return S_OK;
}

//�|���S���̕`��
void CDraw::DrawPolygon()
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	dx.m_pDevice->IASetVertexBuffers(0, 1, &m_pBuffer, &stride, &offset);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	//�l�p�`
	dx.m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�v���~�e�B�u�������_�����O
	D3D10_TECHNIQUE_DESC dc;
	g_Shader.m_pTechnique->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		g_Shader.m_pTechnique->GetPassByIndex(p)->Apply(0);
		dx.m_pDevice->Draw(4, 0);
	}
}

//�e�N�X�`���ǂݍ���
void CDraw::LoadTexture(int Id,const char* Name,int Width,int Height)
{
	ID3D10ShaderResourceView* pTex;
	//�e�N�X�`���[���쐬
	D3DX10CreateShaderResourceViewFromFileA(dx.m_pDevice, Name, NULL, NULL, &pTex, NULL);

	//�o�^
	g_Task.Insert(pTex, Id,Width,Height);
}

//���
void CDraw::Release()
{
	m_pBuffer->Release();
}
