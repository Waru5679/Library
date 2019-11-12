#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include <stdio.h>

constexpr int TRIANGLE_POLYGON{ 3 };	//�O�p�|���S��
constexpr int QUAD_POLYGON{ 4 };		//�l�p�|���S��
constexpr int READ_ARRAY_SIZE{ 200 };	//�ǂݍ��ݗp�L�[�z��̃T�C�Y
constexpr int NAME_ARRAY_SIZE{ 100 };	//���O�z��̃T�C�Y

constexpr int MAX_VER_WEIGH{ 4 };		//���_�̍ő�E�F�C�g

//���_�\����
struct VERTEX
{
	VERTEX()
	{
		for (int i = 0; i < MAX_VER_WEIGH; i++)
		{
			m_BoneIndex[i] = -1;
			m_fWeight[i] = 0.0f;
		}
		m_WeightNum = 0;
	}

	D3DXVECTOR3 m_vPos;	//���_�ʒu
	D3DXVECTOR3 m_vNorm;//���_�@��
	D3DXVECTOR2 m_vTex;	//UV���W

	int m_BoneIndex[MAX_VER_WEIGH];	//�e�����󂯂�{�[���̃C���f�b�N�X
	float m_fWeight[MAX_VER_WEIGH];	//�E�F�C�g
	int m_WeightNum;				//�E�F�C�g��
};

//�|���S��
struct FACE
{
	int m_FaceOfVer;	//�\�����钸�_�̐�
	int* m_pIndex;		//�\�����钸�_�̃C���f�b�N�X
	int m_UseMaterial;	//�g�p����}�e���A��
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
	int*						m_pFaceIndex;				//���̃}�e���A���Ŏg�p����ʂ̃C���f�b�N�X���X�g
	ID3D10Buffer**				m_ppIndexBuffer;			//�C���f�b�N�X�o�b�t�@
};

//�{�[���\����
struct BONE
{
	BONE()
	{
		m_pChildIndex = nullptr;
		D3DXMatrixIdentity(&m_matBindPose);
		D3DXMatrixIdentity(&m_matNewPose);
		D3DXMatrixIdentity(&m_matOffset);
	}
	char		m_Name[NAME_ARRAY_SIZE];	//�{�[����
	int			m_index;					//���g�̃C���f�b�N�X
	int			m_ChildNum;					//�q�̐�
	int*		m_pChildIndex;				//�����̎q�̃C���f�b�N�X���X�g
	D3DXMATRIX	m_matBindPose;				//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX	m_matNewPose;				//���݂̃|�[�Y�i���̓s�x�ς��j
	D3DXMATRIX  m_matOffset;				//�I�t�Z�b�g�s��
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

//�A�j���[�V�����̃L�[
struct KEY
{
	int		m_Time;		//�R�}
	int		m_ValueNum;	//�l�̐�
	float*	m_pfValue;	//�l�̃��X�g
};

//�{�[���̃L�[
struct BONE_KEY
{
	char	m_AffectBoneName[NAME_ARRAY_SIZE];	//�e�����󂯂�{�[����
	int		m_KeyType;							//�L�[�^�C�v
	int		m_KeyNum;							//�L�[�̐�
	KEY*	m_pKey;								//�L�[���X�g
};

//�A�j���[�V�����\����
struct ANIMATION
{
	char		m_Name[NAME_ARRAY_SIZE];//�A�j���[�V������
	int			m_BoneKeyNum;			//�Ή��{�[���̐�
	BONE_KEY*	m_pBoneKey;				//�{�[���̃L�[���X�g
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
	VERTEX*			m_pVertex;		//���_���̃��X�g
};

//�X�L�����b�V���w�b�_�[
struct SKIN_MESH_HEADER
{
	int	m_MaxVertex;//���_�̍ő�E�F�C�g��
	int m_MaxFace;	//�ʂ̍ő�E�F�C�g��
	int m_BoneNum;	//�{�[����
};

//�X�L�����b�V��
struct SKIN_MESH
{

	MESH				m_Mesh;			//���b�V��
	int					m_BoneNum;		//�{�[����
	BONE*				m_pBone;		//�{�[�����X�g
	SKIN_MESH_HEADER	m_SkinHeader;	//�X�L�����b�V���w�b�_�[
	int					m_WeightNum;	//�E�F�C�g��
	SKIN_WEIGHT*		m_pWeight;		//�X�L���E�F�C�g
	int					m_AnimeNum;		//�A�j���[�V������
	ANIMATION*			m_pAnimation;	//�A�j���[�V����
	BONE*				m_pRoot;		//���[�g�{�[��
	D3DXMATRIX			m_mFinalWorld;	//�ŏI�I�ȃ��[���h�s��i���̎p���Ń����_�����O����j
	
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

	//�X�L�����b�V���̓ǂݍ���
	bool LoadSkinMesh(const char* FileName, SKIN_MESH* pSkinMesh);

	//�A�j���[�V����
	void Animation(int AnimeId, int NowFrame, SKIN_MESH* pSkinMesh);

	//���b�V���`��(�e�X�g�p)
	void DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor);
private:
	int GetBoneNum(FILE* fp, long lStartPos);		//�{�[�����̎擾
	int GetSkinWeightNum(FILE* fp, long lStartPos);	//�X�L���E�F�C�g�̐����擾
	int GetAnimeNum(FILE* fp, long lStartPos);		//�A�j���[�V�����̐����擾

	//template���΂����ǂݍ��݊J�n�ʒu���擾����
	long GetTemplateSkipStartPos(FILE* fp);

	//���b�V�����̓ǂݍ���
	bool LoadMesh(FILE* fp, MESH* pMesh, SKIN_MESH_HEADER* pSkinHeader, long lStartPos);

	//�X�L�����b�V���w�b�_�[�ǂݍ���
	void LoadSkinMeshHeader(FILE* fp, SKIN_MESH_HEADER* pSkinHeader, long lStartPos);
	
	//�{�[���ǂݍ���
	bool LoadBone(FILE* fp, BONE* pBone, long lStartPos);

	//�{�[�����̓ǂݍ���(�ċN�֐�))
	BONE LoadBoneInfo(FILE* fp, int* pBoneIndex, BONE* pBone);
	
	//�X�L���E�F�C�g�̓ǂݍ���
	bool LoadSkinWeight(FILE* fp, SKIN_WEIGHT* pSkinWeight, long lStartPos);

	//�A�j���[�V�����ǂݍ���
	bool LoadAnimation(FILE* fp, ANIMATION* pAnime, long lStartPos);

	//�w�蕶���𕶎��񂩂����
	void ErasCharFromString(char* pSource, int Size, char Erace);

	//�X�L�����b�V���ɂ܂Ƃ߂�
	void SkinMeshPutTogether(MESH Mesh, BONE* pBone, int BoneNum,SKIN_WEIGHT* pSkinWeight,int WeightNum,ANIMATION* pAnimation, int AnimeNum, SKIN_MESH* pSkinMesh,SKIN_MESH_HEADER SkinHeader);

	//�{�[�����̃L�[���ǂݍ���
	BONE_KEY LoadBoneKey(FILE* fp);
	
	//�X�L���E�F�C�g�̏������ƂɊe���_�ɑΉ��{�[���ƃE�F�C�g�̏�����������
	void VertexMatchBone(SKIN_MESH* pSkin);

	//�{�[���̍X�V
	void BoneUpdate(SKIN_MESH* pSkin, int AnimeId, int NowFrame);

	//�|�[�Y���擾����
	//D3DXMATRIX GetPose(SKIN_MESH* pSkin, BONE* pBone, ANIMATION Anime, int NowFrame, int BoneID);

	D3DXMATRIX GetPose(bool* bFind, SKIN_MESH* pSkin, BONE* pBone, ANIMATION Anime, int NowFrame, int BoneID);

	//�E�F�C�g���傫�����Ƀ\�[�g����
	void WeightSort(SKIN_MESH* pSkin);

	//�t���[���⊮
	KEY FrameComplement(int NowFrame, BONE_KEY BoneKey);
	
	SKIN_MESH	m_SkinMesh;		//�X�L�����b�V��
};