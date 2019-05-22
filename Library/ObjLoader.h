#pragma once

#include <d3dx10.h>
#include <vector>

using namespace std;

//���_�\����
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//�ʏ��
struct FACE_INFO
{
	int FaceofVer;		//�ʂ��\�����钸�_�̐�
	MY_VERTEX* pVer;	//���_�f�[�^
};

//�}�e���A���\����
struct MY_MATERIAL
{
	//�}�e���A����
	CHAR szName[100];

	D3DXVECTOR3 Ka;//�A���r�G���g(����)
	D3DXVECTOR3 Kd;//�f�B�t���[�Y(�g�U��)
	D3DXVECTOR3 Ks;//�X�y�L�����[(���ʔ��ˌ��j
	
	//�ʏ��
	vector<FACE_INFO> FaceInfo;

	//�ʂ̒��_�̐�
	vector<int>FaceOfVer;

	//�t�@�C�X���
	vector<int>iFaceBuffer;

	//�ʂ̒��_���
//	vector <MY_VERTEX*> FaceVertex;

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

	int Id;//�Ǘ��p��ID
};

//OBJ�f�[�^�ǂݍ��݃N���X
class CObjLoader
{
public:

	//���
	void Release();

	//Mesh�擾
	MY_MESH* GetMesh(int Id);

	//Mesh�̓ǂݍ���
	void LoadMesh(int Id,const char* Name);

	//���b�V���`��(�`��F�w��Ȃ�)
	void Draw(D3DMATRIX matWorld, MY_MESH* pMesh);

	//���b�V���`��(�`��F�w�肠��)
	void Draw(D3DMATRIX matWorld, MY_MESH* pMesh,float fColor[4]);

	//�e�N�X�`�����w�肵�ă��b�V���`��
	void Draw(int TexId,D3DMATRIX matWorld, MY_MESH* pMesh);

	//OBJ�̓ǂݍ���
	HRESULT LoadObj(const char* FileName, MY_MESH* pMesh);

private:

	//�}�e���A���̓ǂݍ���
	HRESULT LoadMaterial(char* FileName, MY_MESH* pMesh);

	//�ő�ƍŏ��̃`�F�b�N
	void MinAndMax(D3DXVECTOR3 Pos,MY_MESH* pMesh);

	//�|���S���`��
	void DrawMesh(int ver_num, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer);

	//���b�V��
	vector<MY_MESH>m_Mesh;
};

extern CObjLoader g_Loader;