#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include <stdio.h>

constexpr int TRIANGLE_POLYGON{ 3 };	//�O�p�|���S��
constexpr int QUAD_POLYGON{ 4 };		//�l�p�|���S��
constexpr int READ_ARRAY_SIZE{ 200 };	//�ǂݍ��ݗp�L�[�z��̃T�C�Y
constexpr int NAME_ARRAY_SIZE{ 100 };	//���O�z��̃T�C�Y

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
	char						m_TexName[NAME_ARRAY_SIZE];	//�t�@�C����
	D3DXVECTOR4					m_vFaceColor;				//�ʂ̐F
	D3DXVECTOR3					m_vKs;						//�X�y�L�����[
	float						m_fPower;					//�X�y�L�����[�̃p���[
	D3DXVECTOR3					m_vKe;						//�G�~�b�V�u
	ID3D10ShaderResourceView*	m_pTexture;					//�e�N�X�`���|�C���^	
	int							m_FaceNum;					//���̃}�e���A�����g�p����ʂ̐�
	int*						m_pVerNum;					//���_���̃��X�g
	ID3D10Buffer**				m_ppIndexBuffer;			//�C���f�b�N�X�o�b�t�@
};

//�{�[���\����
struct BONE
{
	char		m_Name[NAME_ARRAY_SIZE];	//�{�[����
	int			m_index;					//���g�̃C���f�b�N�X
	int			m_ChildNum;					//�q�̐�
	int*		m_pChildIndex;				//�����̎q�̃C���f�b�N�X���X�g
	D3DXMATRIX	m_matBindPose;				//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX	m_matNewPose;				//���݂̃|�[�Y�i���̓s�x�ς��j
};

//�ǂ̃{�[�����x�̒��_�ɂǂꂾ���̉e����^���邩
struct SKIN_WEIGHT
{
	char		m_BoneName[NAME_ARRAY_SIZE];//�{�[����
	int			m_WeightNum;				//�E�F�C�g�̐�
	int*		m_pIndex;					//�e�����󂯂钸�_�̃C���f�b�N�X���X�g
	float*		m_pWeight;					//�E�F�C�g���X�g
	D3DXMATRIX	m_matOffset;				//�I�t�Z�b�g�s��
};

//�X�L���E�F�C�g�����������{�[��
struct SKIN_BONE
{
	char		m_Name[NAME_ARRAY_SIZE];//�{�[����
	int			m_index;				//���g�̃C���f�b�N�X
	int			m_ChildNum;				//�q�̐�
	int*		m_pChildIndex;			//�����̎q�̃C���f�b�N�X���X�g
	int			m_WeightNum;			//�E�F�C�g�̐�
	int*		m_pIndex;				//�e����^���钸�_�C���f�b�N�X���X�g
	float*		m_pWeight;				//�E�F�C�g���X�g
	D3DXMATRIX	m_matOffset;			//�I�t�Z�b�g�s��
	D3DXMATRIX	m_matBindPose;			//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX	m_matNewPose;			//���݂̃|�[�Y�i���̓s�x�ς��j
};

//�A�j���[�V�����̃L�[
struct KEY
{
	int		m_Time;		//�R�}
	int		m_ValueNum;	//�l�̐�
	float*	m_pValue;	//�l�̃��X�g
};

//�A�j���[�V�����\����
struct ANIMATION
{
	char	m_Name[NAME_ARRAY_SIZE];			//�A�j���[�V������
	char	m_AffectBoneName[NAME_ARRAY_SIZE];	//�e�����󂯂�{�[����
	int		m_KeyType;							//�L�[�^�C�v
	int		m_KeyNum;							//�L�[�̐�
	KEY*	m_pKey;								//�L�[���X�g
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

	SKIN_BONE* m_pSkinBone;	//�X�L���������������{�[��
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
	
	bool LoadMesh(FILE* fp, MESH* pMesh,long lStartPos);	//���b�V�����̓ǂݍ���
	
	bool LoadBone(FILE* fp, SKIN_MESH* pSkinMesh,long lStartPos);	//�{�[���ǂݍ���
	BONE LoadBoneInfo(FILE* fp, int* pBoneIndex, SKIN_MESH* pSkinMesh);//�{�[�����̓ǂݍ���(�ċN�֐�))

	bool LoadSkin(FILE* fp, SKIN_MESH* pSkinMesh, long lStartPos);	//�X�L�����̓ǂݍ���

	void ErasCharFromString(char* pSource,int Size, char Erace);//�w�蕶���𕶎��񂩂����

	long GetTemplateSkipStartPos(FILE* fp);	//template���΂����ǂݍ��݊J�n�ʒu���擾����

	bool LoadAnimation(FILE* fp, SKIN_MESH* pSkinMesh, long lStartPos);//�A�j���[�V�����ǂݍ���

	
	void BoneWithSkin(SKIN_MESH* pSkinMesh);//�{�[���ƃX�L�������܂Ƃ߂�


	//���b�V���`��(�e�X�g�p)
	void DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor);
	
private:

	SKIN_MESH	m_SkinMesh;		//�X�L�����b�V��
};