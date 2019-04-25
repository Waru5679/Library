#include "Polygon.h"
#include "DirectX.h"
#include "Task.h"
#include "Shader.h"
#include "Camera.h"
#include "Math.h"

CDraw g_Draw;

//������
void CDraw::Init()
{
	//���_���
	MY_VERTEX ver[]=
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),
		D3DXVECTOR3(0.0f, -0.1f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),
		D3DXVECTOR3(0.1f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),
		D3DXVECTOR3(0.1f, -0.1f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f)
	};
	m_Vertex = ver;

	//�o�[�e�b�N�X�o�b�t�@�쐬
	CreateBartex();
}

//�`�� ���p
void CDraw::Draw3D(int TexId, D3DXMATRIX matWorld) 
{	
	Draw3D(g_Task.GetTex(TexId), matWorld);
}

//�`��
void CDraw::Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld)
{
	//�V�F�[�_�[�̃Z�b�g
	g_Shader.SetShader(pResView,matWorld);

	//�|���S���̕`��
	DrawPolygon();
}

//2D�`�� ���p
void CDraw::Draw2D(int TexId, RECT_F* Out, D3DXMATRIX matWorld)
{
	Draw2D(g_Task.GetTex(TexId),Out,matWorld);
}

//2D�`��
void CDraw::Draw2D(ID3D10ShaderResourceView* pResView, RECT_F*Out,D3DXMATRIX matWorld)
{
	CalcScreenToWorld(&m_vPos, 0.0f, 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT,
		&m_pCamera->GetViewMatrix(), &m_pCamera->GetProjMatrix());


	m_vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);


	//�t�r���[�s��
	D3DXMATRIX matInvView;	
	D3DXMatrixIdentity(&matInvView);
	D3DXMatrixInverse(&matInvView,NULL,& m_pCamera->GetViewMatrix());

	//�t�v���W�F�N�V�����s��
	D3DXMATRIX matInvProj;
	D3DXMatrixIdentity(&matInvProj);
	D3DXMatrixInverse(&matInvProj, NULL, &m_pCamera->GetProjMatrix());

	//���s�ړ�����
	matWorld._41 = 0.0f;
	matWorld._42 = 0.0f;
	matWorld._43 = 0.0f;

	D3DXMATRIX matOrtho;
	D3DXMatrixIdentity(&matOrtho);
	matOrtho._11 = 2 / WINDOW_WIDTH;
	matOrtho._22 = -2 / WINDOW_HEIGHT;
	matOrtho._41 = -1;
	matOrtho._42 = 1;


		
	//�V�F�[�_�[�̃Z�b�g
	g_Shader.SetShader(pResView, matWorld *matInvProj*matInvView*matOrtho);

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
void CDraw::LoadTexture(int Id,const char* Name)
{
	ID3D10ShaderResourceView* pTex;
	//�e�N�X�`���[���쐬
	D3DX10CreateShaderResourceViewFromFileA(dx.m_pDevice, Name, NULL, NULL, &pTex, NULL);

	//�o�^
	g_Task.Insert(pTex, Id);
}

//���
void CDraw::Release()
{
	m_pBuffer->Release();
}

//RECT_F�̃Z�b�g
void RectSet(float x, float y, float Size_x, float Size_y, RECT_F* pOut)
{
	pOut->m_top = y;
	pOut->m_bottom = pOut->m_top + Size_y;
	pOut->m_left = x;
	pOut->m_right = pOut->m_left + Size_x;
}