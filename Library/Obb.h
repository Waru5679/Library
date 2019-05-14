#ifndef	_NewOBB
#define	_NewOBB

#include "Main.h"
#include "CObj.h"
#include <D3D10.h>
#include <vector>
using namespace std;

//OBB�\����
struct OBB_Data
{
public:
	D3DXVECTOR3 Norm[6];	//�ʂ̖@��
	D3DXVECTOR3 VerPos[8];	//���_�̈ʒu
	D3DXVECTOR3 m_Pos;      //�ʒu
	D3DXVECTOR3 m_Direct[3];//�����x�N�g��
	FLOAT		m_Length[3];//�e�������̒���
	D3DXVECTOR3 vMin;		//Mesh�ŏ��l
	D3DXVECTOR3 vMax;		//Mesh�ő�l
	D3DXMATRIX matWorld;
	int Name;
	void* p;
	
	D3DXVECTOR3 m_Angle;
	D3DXVECTOR3 m_Scale;
};

//�q�b�g�f�[�^
struct HIT_DATA
{
	D3DXVECTOR3 vDir;	//����
	D3DXVECTOR3 vnDir;	//����
	float fDis;			//����

	//�I�u�W�F�N�g�T�C�Y
	D3DXVECTOR3 vMin;	
	D3DXVECTOR3 vMax;
	void* p;

	OBB_Data* obb;
};

//OBB�N���X
class COBB
{
public:

	//������
	void Init();
	
	//OBB�Z�b�g
	OBB_Data SetOBB(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale, D3DXVECTOR3 Min, D3DXVECTOR3 Max, int ObjName, void* p);

	//OBB�o�^
	void Insert(OBB_Data* obb);

	//OBB�X�V
	void Update(OBB_Data* obb, D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale, D3DXVECTOR3 Min, D3DXVECTOR3 Max);

	//OBB�̕`��
	void Draw();

	//�q�b�g�`�F�b�N
	vector<HIT_DATA> ObjNameHit(OBB_Data* Obb,int Name);

	//�߂荞�ݏC��
	D3DXVECTOR3 NestingCorrection(OBB_Data* StopObb,OBB_Data* MoveObb);
private:
	//�Փ˔���
	bool Collision(OBB_Data* obb1, OBB_Data* obb2);

	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 );

	//OBB�f�[�^
	vector<OBB_Data*> m_Data;

	//OBB�\���p���b�V��
	MY_MESH m_Mesh;

	//�`��F
	float m_fColor[4];
};

extern COBB g_Obb;

#endif

