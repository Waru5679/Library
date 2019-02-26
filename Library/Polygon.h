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
typedef class CDrawPolygon
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
	
private:
	void DrawPolygon();		//�|���S���`��
	HRESULT CreateBartex();	//�o�[�e�b�N�X�o�b�t�@�̍쐬


	MY_MATERIAL m_material;	//�}�e���A��
	MY_VERTEX* m_vertex;	//���_
	ID3D10Buffer* m_pBuffer;//�o�[�e�b�N�X�o�b�t�@
	
	D3DXVECTOR3 m_vPos;		//�ʒu
	D3DXVECTOR3 m_vAngle;	//��]
	D3DXVECTOR3 m_vScale;	//�傫��

	D3DXMATRIX m_matWorld;	//���[���h�s��

}CDraw;

extern CDraw g_Draw;