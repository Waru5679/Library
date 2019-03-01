#pragma once

#include "Main.h"
#include "ObjLoader.h"
#include <windef.h>

struct RECT_F
{
	float m_top;
	float m_left;
	float m_right;
	float m_bottom;
};

//�`��N���X
class CDraw
{
public:
	//������
	void Init();

	//�`��
	void Draw3D(int TexId, D3DXMATRIX matWorld);
	void Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld);
	void Draw2D(int TexId,RECT_F* Out);
	void Draw2D(ID3D10ShaderResourceView* pResView,RECT_F*Out);

	//�e�N�X�`���ǂݍ���
	void LoadTexture(int Id, const char* Name);

	//�J�����Z�b�g
	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }

	//���
	void Release();
	
private:
	void DrawPolygon();		//�|���S���`��
	HRESULT CreateBartex();	//�o�[�e�b�N�X�o�b�t�@�̍쐬
	
	ID3D10Buffer* m_pBuffer;//�o�[�e�b�N�X�o�b�t�@

	MY_MATERIAL m_Material;	//�}�e���A��
	MY_VERTEX* m_Vertex;	//���_
	
	D3DXVECTOR3 m_vPos;		//�ʒu
	D3DXVECTOR3 m_vAngle;	//��]
	D3DXVECTOR3 m_vScale;	//�傫��

	D3DXMATRIX m_matWorld;	//���[���h�s��
	
	Camera* m_pCamera;	//�J�����|�C���^

};

extern CDraw g_Draw;