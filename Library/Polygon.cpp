#include "Polygon.h"
#include "DirectX.h"
#include "Task.h"
#include "Shader.h"
#include "Camera.h"
#include "Math.h"

CDraw g_Draw;

//������
void CDrawPolygon::Init()
{
	//���_���
	MY_VERTEX ver[]=
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),
		D3DXVECTOR3(0.0f, -0.1f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),
		D3DXVECTOR3(0.1f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),
		D3DXVECTOR3(0.1f, -0.1f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f)
	};
	m_vertex = ver;

	//�o�[�e�b�N�X�o�b�t�@�쐬
	CreateBartex();

	//�}�e���A��
	m_material.Ka = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	m_material.Kd = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	m_material.Ks = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

}

//�`�� ���p
void CDrawPolygon::Draw3D(int TexId, D3DXMATRIX matWorld) 
{	
	Draw3D(g_Task.GetTex(TexId), matWorld);
}

//�`��
void CDrawPolygon::Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld)
{
	//�e�N�X�`��
	m_material.pTexture = pResView;

	//���[���h�s��
	m_matWorld = matWorld;

	//�V�F�[�_�[�̃Z�b�g
	g_Shader.SetShader(matWorld, m_material);

	//�|���S���̕`��
	DrawPolygon();
}

//2D�`�� ���p
void CDrawPolygon::Draw2D(int TexId, RECT_F* Out)
{

	Draw2D(g_Task.GetTex(TexId),Out);
}

//2D�`��
void CDrawPolygon::Draw2D(ID3D10ShaderResourceView* pResView, RECT_F*Out)
{
	//�e�N�X�`��
	m_material.pTexture = pResView;

	//�X�N���[�����W����ʒu���擾
	Camera* pCamera = g_Task.GetCamera(0);
	CalcScreenToWorld(&m_vPos, Out->m_left, Out->m_top, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, &pCamera->GetViewMatrix(), &pCamera->GetProjMatrix());
	
	//�X�P�[��
	m_vScale.x = (Out->m_right  - Out->m_left) / 240.0f;
	m_vScale.y = (Out->m_bottom - Out->m_top)/ 240.0f;
	m_vScale.z = 0.0f;

	//�J�����̉�]���擾
	m_vAngle= pCamera->GetVertical();
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
	   
	//�V�F�[�_�[�̃Z�b�g
	g_Shader.SetShader(m_matWorld, m_material);

	//�|���S���̕`��
	DrawPolygon();
}

//�o�[�e�b�N�X�o�b�t�@�[�쐬
HRESULT CDrawPolygon::CreateBartex()
{
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_VERTEX) * 4;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_vertex;
	if (FAILED(dx.m_pDevice->CreateBuffer(&bd, &InitData, &m_pBuffer)))
		return FALSE;

	return S_OK;
}

//�|���S���̕`��
void CDrawPolygon::DrawPolygon()
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
	dx.m_pTechnique->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		dx.m_pTechnique->GetPassByIndex(p)->Apply(0);
		dx.m_pDevice->Draw(4, 0);
	}
}

//�e�N�X�`���ǂݍ���
void CDrawPolygon::LoadTexture(int Id,const char* Name)
{
	ID3D10ShaderResourceView* pTex;
	//�e�N�X�`���[���쐬
	D3DX10CreateShaderResourceViewFromFileA(dx.m_pDevice, Name, NULL, NULL, &pTex, NULL);

	//�o�^
	g_Task.Insert(pTex, Id);
}