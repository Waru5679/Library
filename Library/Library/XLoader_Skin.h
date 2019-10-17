#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"

#include <stdio.h>

//���_�\����
struct VERTEX
{
	D3DXVECTOR3 m_vPos;	//���_�ʒu
	D3DXVECTOR3 m_vNorm;//���_�@��
	D3DXVECTOR2 m_vTex;	//UV���W
};

//�|���S��
struct FACE
{
	int m_FaceOfVer;	//�\�����钸�_�̐�
	int* m_pIndex;		//�\�����钸�_�̃C���f�b�N�X
};

//�}�e���A���\����
struct MATERIAL
{
	D3DXVECTOR4					m_vFaceColor;	//�ʂ̐F
	D3DXVECTOR3					m_vKs;			//�X�y�L�����[
	float						m_fPower;		//�X�y�L�����[�̃p���[
	D3DXVECTOR3					m_vKe;			//�G�~�b�V�u
	char						m_TexName[100];	//�t�@�C����
	ID3D10ShaderResourceView*	m_pTexture;		//�e�N�X�`���|�C���^	
	int							m_FaceNum;		//���̃}�e���A�����g�p����ʂ̐�
	int*						m_pVerNum;		//���_���̃��X�g
	ID3D10Buffer**				m_ppIndexBuffer;//�C���f�b�N�X�o�b�t�@
};

//�{�[���\����
struct BONE
{
	char		m_Name[100];	//�{�[����
	int			m_index;		//���g�̃C���f�b�N�X
	int			m_ChildNum;		//�q�̐�
	int*		m_pChildIndex;	//�����̎q�̃C���f�b�N�X���X�g
	D3DXMATRIX	m_matBindPose;	//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX	m_matNewPose;	//���݂̃|�[�Y�i���̓s�x�ς��j
};

//�ǂ̃{�[�����ǂ̃{�[���ɂǂꂾ���̉e����^���邩
struct SKIN_WEIGHT
{
	char		m_BoneName[100];//�{�[����
	int			m_WeightNum;	//�E�F�C�g�̐�
	int*		m_pIndex;		//�e�����󂯂钸�_�̃C���f�b�N�X���X�g
	float*		m_pWeight;		//�E�F�C�g���X�g
	D3DXMATRIX	m_matOffset;	//�I�t�Z�b�g�s��
};

//�A�j���[�V�����̃L�[
struct Key
{
	int		m_Time;		//�R�}
	int		m_ValueNum;	//�l�̐�
	float* m_pValue;	//�l�̃��X�g
};

//�A�j���[�V�����\����
struct ANIMATION
{
	char	m_Name[100];			//�A�j���[�V������
	char	m_AffectBoneName[100];	//�e�����󂯂�{�[����
	int		m_KeyType;				//�L�[�^�C�v
	int		m_KeyNum;				//�L�[�̐�
	Key*	m_pKey;					//�L�[���X�g
};

//���b�V��
struct MESH
{
	int				m_VerNum;		//���_��
	int				m_UvNum;		//UV�̐�(���_���Ɠ���)
	int				m_FaceNum;		//��(�|���S��)��
	FACE*			m_pFace;		//�ʃ��X�g
	int				m_MaterialNum;	//�}�e���A���̐�
	MATERIAL*		m_pMaterial;	//�}�e���A�����X�g
	ID3D10Buffer*	m_pVertexBuffer;//���_�o�b�t�@
};

//�X�L�����b�V��
struct SKIN_MESH
{
	MESH		m_Mesh;			//���b�V��
	int			m_BoneNum;		//�{�[����
	BONE*		m_pBone;		//�{�[�����X�g	
	D3DXMATRIX	m_mFinalWorld;	//�ŏI�I�ȃ��[���h�s��i���̎p���Ń����_�����O����j
	int			m_WeightNum;	//�E�F�C�g��
	SKIN_WEIGHT*m_pSkinWeight;	//�X�L���E�F�C�g
	int			m_AnimeNum;		//�A�j���[�V������
	ANIMATION*	m_pAnimation;	//�A�j���[�V����
};

//X�t�@�C���֘A�̃N���X
class CX_Skin : public CSingleton<CX_Skin>
{
private:
	//�V���O���g��
	friend CSingleton<CX_Skin>;
	CX_Skin() {};	//�R���X�g���N�^
	~CX_Skin() {};	//�f�X�g���N�^
public:
	void Release();//�J��
	bool LoadSkinMesh(const char* FileName, SKIN_MESH* pSkinMesh);	//�X�L�����b�V���̓ǂݍ���
	bool LoadMesh(FILE* fp, MESH* pMesh);	//���b�V�����̓ǂݍ���
	
	bool LoadBone(FILE* fp, SKIN_MESH* pSkinMesh);	//�{�[���ǂݍ���
	BONE LoadBoneInfo(FILE* fp, int* pBoneIndex, SKIN_MESH* pSkinMesh);//�{�[�����̓ǂݍ���
	//���b�V���`��(�e�X�g�p)
	void DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor);
	
private:

	SKIN_MESH	m_SkinMesh;		//�X�L�����b�V��
};