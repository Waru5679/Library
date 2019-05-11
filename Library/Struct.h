#include"Main.h"
#include <vector>
using namespace std;


//RECT_F
struct RECT_F
{
	float m_top;
	float m_left;
	float m_right;
	float m_bottom;
};

//���_�\����
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//�}�e���A���\����
struct MY_MATERIAL
{
	//�}�e���A����
	CHAR szName[100];

	D3DXVECTOR3 Ka;//�A���r�G���g(����)
	D3DXVECTOR3 Kd;//�f�B�t���[�Y(�g�U��)
	D3DXVECTOR3 Ks;//�X�y�L�����[(���ʔ��ˌ��j

	//�ʂ̒��_�̐�
	vector<int>FaceOfVer;

	//�t�@�C�X���
	vector<int>iFaceBuffer;

	//�e�N�X�`��
	ID3D10ShaderResourceView* pTexture;

	//�o�b�t�@�[
	vector<ID3D10Buffer*> pIndex;
	vector<ID3D10Buffer*> pVertex;
};

//�I���W�i�����b�V��
struct MY_MESH
{
	vector<MY_MATERIAL> Material;	//�}�e���A��
	int MaterialNum;				//�}�e���A���̐�

	D3DXVECTOR3 vMin;	//���_�̍ŏ����W
	D3DXVECTOR3 vMax;	//���_�̍ő���W
};
