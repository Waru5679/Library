#pragma once

#include "Main.h"
#include "Camera.h"
#include "Struct.h"

//�`��N���X
class CDraw
{
public:	
	void Init();	//������
	void Release();	//���

	//�o�b�t�@�쐬
	ID3D10Buffer* BufferCreate(void* pHead, unsigned int Size, int BufferType);

	//2D�`��(�e�N�X�`���̃T�C�Y����؂���ʒu�̐ݒ肪������̗p���p)
	void DrawTexture(int TexId, RECT_F* pSrc, RECT_F* pOut, ColorData* pColor, float fRad);

	//2D�`��
	void DrawTexture(ID3D10ShaderResourceView* pTex, RECT_F* pSrc, RECT_F* pOut, ColorData* pColor, float fRad);
	
	//�e�N�X�`���ǂݍ���
	void LoadTexture(int Id, const char* Name,int Width, int Height);

	//�J�����Z�b�g
	void SetCamera(CameraBase* pCamera) { m_pCamera = pCamera; }

	//�|���S���`��
	void DrawPolygon(int VerNum, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer);
private:
	
	ID3D10Buffer* m_pVertexBuffer;//�o�[�e�b�N�X�o�b�t�@
	  	
	CameraBase* m_pCamera;	//�J�����|�C���^
};

extern CDraw g_Draw;