#pragma once
#include "Singleton.h"
#include <stdio.h>

struct	BDEF1
{
	BDEF1()
	{
		m_BoneID = -1;
	}
	int m_BoneID;
};

struct	BDEF2
{
	BDEF2()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
	}
	int m_BoneID[2];
	float m_Weight;
};

struct	BDEF4
{
	BDEF4()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
		m_BoneID[2] = -1;
		m_BoneID[3] = -1;
	}

	int m_BoneID[4];
	float m_Weight[4];
};

struct SDEF
{
	SDEF()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
	}
	int m_BoneID[2];
	float m_Weight;
	float m_Matrix[3][3];
};

//�E�F�C�g�f�[�^
struct PMX_WEIGHT_DATA
{
	PMX_WEIGHT_DATA()
	{
		m_pBdef1 = nullptr;
		m_pBdef2 = nullptr;
		m_pBdef4 = nullptr;
		m_pSdef = nullptr;
	}
	~PMX_WEIGHT_DATA()
	{
		//BDEF1
		if (m_pBdef1 != nullptr)
		{
			delete m_pBdef1;
			m_pBdef1 = nullptr;
		}
		//BDEF2
		if (m_pBdef2 != nullptr)
		{
			delete m_pBdef2;
			m_pBdef2 = nullptr;
		}
		//BDEF4
		if (m_pBdef4 != nullptr)
		{
			delete m_pBdef4;
			m_pBdef4 = nullptr;
		}
		//SDEF
		if (m_pBdef1 != nullptr)
		{
			delete m_pSdef;
			m_pSdef = nullptr;
		}
	}
	unsigned char m_WeightType;//�E�F�C�g�ό`����
	
	BDEF1*	m_pBdef1;	//BDEF1�`���̃f�[�^
	BDEF2*	m_pBdef2;	//BDEF2�`���̃f�[�^
	BDEF4*	m_pBdef4;	//BDEF4�`���̃f�[�^
	SDEF*	m_pSdef;	//SDEF�`���̃f�[�^
};

//pmx�w�b�_�[
struct PMX_HEADER
{
	~PMX_HEADER()
	{
		if (m_pData != nullptr)
		{
			delete[] m_pData;
			m_pData = nullptr;
		}
	}
	char	m_FileType[4];	//�t�@�C�����"PMX_"�Œ�
	char	m_Size;			//�f�[�^�T�C�Y
	float	m_Ver;			//�o�[�W����
	unsigned char*	m_pData;
};

//pmx���f�����
struct PMX_MODEL_INFO
{
	PMX_MODEL_INFO()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
		m_pCommentJap = nullptr;
		m_pCommentEng = nullptr;
	}
	~PMX_MODEL_INFO()
	{
		//���f����(��)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//���f����(�p)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
		//�R�����g(��)
		if (m_pCommentJap != nullptr)
		{
			delete[] m_pCommentJap;
			m_pCommentJap = nullptr;
		}
		//�R�����g(�p)
		if (m_pCommentEng!= nullptr)
		{
			delete[] m_pCommentEng;
			m_pCommentEng = nullptr;
		}
	}
	wchar_t* m_pNameJap;	//���f����(��)
	wchar_t* m_pNameEng;	//���f����(�p)
	wchar_t* m_pCommentJap;	//�R�����g(��)
	wchar_t* m_pCommentEng;	//�R�����g(�p)
};

//pmx���_�f�[�^
struct PMX_VERTEX
{
	PMX_VERTEX()
	{
		m_pfAddUv = nullptr;
	}
	~PMX_VERTEX()
	{
		//�ǉ�UV
		if (m_pfAddUv != nullptr)
		{
			delete[] m_pfAddUv;
			m_pfAddUv = nullptr;
		}
	}

	float m_fPos[3];				//�ʒu
	float m_fNorm[3];				//�@��
	float m_fUv[2];					//UV
	float* m_pfAddUv;				//�ǉ�UV
	float m_EdgeMagn;				//�G�b�W�{��
	
	PMX_WEIGHT_DATA m_WeightData;	//�E�F�C�g�f�[�^
};

//pmx�ʃf�[�^
struct PMX_FACE
{
	int m_VerIndex[3];//���_
};

//pmx�e�N�X�`��
struct PMX_TEXTURE
{
	PMX_TEXTURE()
	{
		m_pPass = nullptr;
	}
	~PMX_TEXTURE()
	{
		//�e�N�X�`���p�X
		if (m_pPass != nullptr)
		{
			delete[] m_pPass;
			m_pPass = nullptr;
		}
	}
	wchar_t* m_pPass;	//�e�N�X�`���p�X
};

//pmx�}�e���A��
struct PMX_MATERIAL
{
	PMX_MATERIAL()
	{
		m_pNameEng = nullptr;
		m_pNameJap = nullptr;
		m_pMemo = nullptr;
	}
	~PMX_MATERIAL()
	{
		//�ގ���(��)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//�ގ���(�p)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
		//����
		if (m_pMemo != nullptr)
		{
			delete[] m_pMemo;
			m_pMemo = nullptr;
		}
	}

	wchar_t* m_pNameJap;	//�ގ���(��)
	wchar_t* m_pNameEng;	//�ގ���(�p)
	wchar_t* m_pMemo;		//����	

	unsigned char	m_BitFlag;		//�`��t���O( 0x01:���ʕ`��, 0x02:�n�ʉe, 0x04:�Z���t�V���h�E�}�b�v�ւ̕`��, 0x08:�Z���t�V���h�E�̕`��, 0x10:�G�b�W�`��)
	unsigned char	m_SphereMode;	//�X�t�B�A���[�h(0:���� 1:��Z(sph) 2:���Z(spa) 3:�T�u�e�N�X�`��)
	unsigned char	m_ToonFlag;		//���LToon�t���O

	float	m_Diffuse[4];	//�g�U��
	float	m_Specular[3];	//�X�y�L�����[�i���ʔ���
	float	m_SpePower;		//�X�y�L�����[�p���[
	float	m_Ambient[3];	//�A���r�G���g
	float	m_Edge[4];		//�G�b�W�F
	float	m_EdgeSize;		//�G�b�W�T�C�Y
	int		m_NormTex;		//�ʏ�e�N�X�`���C���f�b�N�X
	int		m_SphereTex;	//�X�t�B�A�e�N�X�`���C���f�b�N�X
	int		m_ToonTex;		//�g�D�[���e�N�X�`���C���f�b�N�X
	int		m_UseVerNum;	//�g�p���钸�_��
};

//pmxIK�����N
struct PMX_IK_LINK
{
	unsigned char m_RadRest;//�p�x�����̗L��

	int		m_LinkBoneId;	//�����N�{�[��ID
	float	m_fLowerRad[3];	//�����p�x
	float	m_fUpperRad[3];	//����p�x
};

//pmxIK
struct PMX_IK
{
	PMX_IK()
	{
		m_pLink = nullptr;
	}
	~PMX_IK()
	{
		delete[] m_pLink;
		m_pLink = nullptr;
	}
	int		m_TargetId;	//�^�[�Q�b�g�{�[��Id
	int		m_RoopTime;	//��]��
	int		m_LinkNum;	//�����N��

	float	m_fRad;		//��]�p�x

	PMX_IK_LINK* m_pLink;//�����N�f�[�^
};

//pmx�{�[��
struct PMX_BONE
{
	PMX_BONE()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
	}
	~PMX_BONE()
	{
		//�{�[����(��)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//�{�[����(�p)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
	}
	wchar_t*		m_pNameJap;		//�{�[����(��)
	wchar_t*		m_pNameEng;		//�{�[����(�p)
	unsigned char	m_BitFlag[2];	//�r�b�g�t���O
		
	int		m_ParentId;		//�e�C���f�b�N�X
	int		m_Hierarchy;	//�ό`�K�w
	int		m_ConnectId;	//�ڑ���{�[��ID
	int		m_GrantId;		//�t�^�e�{�[��

	float	m_fOffset[3];	//�I�t�Z�b�g
	float	m_fGrantRate;	//�t�^��
	float	m_fPos[3];		//�ʒu
	float	m_fFixedAxis[3];//�Œ莲
	float	m_fAxisX[3];	//X��
	float	m_fAxisZ[3];	//Z��
	int		m_Key;			//Key
	PMX_IK	m_Ik;			//IK�f�[�^
};


//pmx���_���[�t
struct PMX_VER_MORPH
{
	int		m_VerId;		//���_ID
	float	m_fOffset[3];	//���W�I�t�Z�b�g�l
};

//pmxUV���[�t
struct PMX_UV_MORPH
{
	int		m_VerId;		//���_ID
	float	m_fOffset[4];	//UV�I�t�Z�b�g
};

//pmx�{�[�����[�t
struct PMX_BONE_MORPH
{
	int		m_BoneId;	//�{�[��ID
	float	m_fMove[3];	//�ړ���
	float	m_fRot[4];	//��]��
};

//pmx�ގ����[�t
struct PMX_MATE_MORPH
{
	unsigned char m_Format;//���Z�`��

	int		m_MateId;		//�Ή��}�e���A��ID(-1�͑S�}�e���A���Ή�)
	float	m_fDiffuse[4];	//�g�U��
	float	m_fSpecular[3];	//�X�y�L�����[�i���ʔ���
	float	m_fSpePower;		//�X�y�L�����[�p���[
	float	m_fAmbient[3];	//�A���r�G���g
	float	m_fEdge[4];		//�G�b�W�F
	float	m_fEdgeSize;		//�G�b�W�T�C�Y
	float	m_fTex[4];		//�e�N�X�`���W��
	float	m_fSphereTex[4];	//�X�t�B�A�e�N�X�`���W��
	float	m_fToonTex[4];	//Toon�e�N�X�`���W��
};

//�O���[�v���[�t
struct PMX_GROUP_MORPH
{
	int		m_MorphId;	//���[�tID
	float	m_fRate;	//���[�t��
};

//pmx���[�t
struct PMX_MORPH
{
	PMX_MORPH()
	{
		m_pNameJap		= nullptr;
		m_pNameEng		= nullptr;
		m_pVerMorph		= nullptr;	
		m_pUvMorph		= nullptr;
		m_pBoneMorph	= nullptr;
		m_pMateMorph	= nullptr;
		m_pGroupMorph	= nullptr;
	}
	~PMX_MORPH()
	{
		//���[�t��(��)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//���[�t��(�p)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
		//���_���[�t
		if (m_pVerMorph != nullptr)
		{
			delete[] m_pVerMorph;
			m_pVerMorph = nullptr;
		}
		//UV���[�t
		if (m_pUvMorph != nullptr)
		{
			delete[] m_pUvMorph;
			m_pUvMorph = nullptr;
		}
		//�{�[�����[�t
		if (m_pBoneMorph != nullptr)
		{
			delete[] m_pBoneMorph;
			m_pBoneMorph = nullptr;
		}
		//�ގ����[�t
		if (m_pMateMorph != nullptr)
		{
			delete[] m_pMateMorph;
			m_pMateMorph = nullptr;
		}
		//�O���[�v���[�t
		if (m_pGroupMorph != nullptr)
		{
			delete[] m_pGroupMorph;
			m_pGroupMorph = nullptr;
		}
	}

	wchar_t* m_pNameJap;		//���[�t��(��)
	wchar_t* m_pNameEng;		//���[�t��(�p)
	unsigned char m_PmdType;		//PMD�J�e�S���[(0:�V�X�e���\��,1:��(����),2:��(����),3:��(�E��),4:���̑�(�E��)) 
	unsigned char m_MorphType;		//���[�t�^�C�v(0:�O���[�v,1:���_,2:�{�[��,3:UV,4-7:�ǉ�UV,8:�ގ�)
	
	int	m_DataNum;	//�f�[�^��

	PMX_VER_MORPH*		m_pVerMorph;	//���_���[�t
	PMX_UV_MORPH*		m_pUvMorph;		//UV���[�t
	PMX_BONE_MORPH*		m_pBoneMorph;	//�{�[�����[�t
	PMX_MATE_MORPH*		m_pMateMorph;	//�ގ����[�t
	PMX_GROUP_MORPH*	m_pGroupMorph;	//�O���[�v���[�t
};

//�g���v�f
struct FRAME_ELEMENT
{
	unsigned char m_Flag;	//0:�{�[�� 1:���[�t
	int m_Index;			//�{�[���܂��̓��[�t�̃C���f�b�N�X
};

//�\���g
struct PMX_DISPLAY
{
	PMX_DISPLAY()
	{
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
	}
	~PMX_DISPLAY()
	{
		//�\���g��(��)
		if (m_pNameJap != nullptr)
		{
			delete[] m_pNameJap;
			m_pNameJap = nullptr;
		}
		//�\���g��(�p)
		if (m_pNameEng != nullptr)
		{
			delete[] m_pNameEng;
			m_pNameEng = nullptr;
		}
		//�g���v�f��
		if (m_pElement != nullptr)
		{
			delete[] m_pElement;
			m_pElement = nullptr;
		}
	}
	wchar_t* m_pNameJap;		//�\���g��(��)
	wchar_t* m_pNameEng;		//�\���g��(�p)

	unsigned char	m_SpecialFlag;	//����t���O
	int				m_ElementNum;	//�v�f��
	FRAME_ELEMENT*	m_pElement;		//�g���v�f��
};

//pmx�f�[�^
struct PMX_DATA
{
	PMX_DATA()
	{	
		m_pVertex	= nullptr;//���_�f�[�^
		m_pFace		= nullptr;//�ʂ̃f�[�^
		m_pTex		= nullptr;//�e�N�X�`���f�[�^
		m_pMaterial = nullptr;//�}�e���A���f�[�^
		m_pBone		= nullptr;//�{�[���f�[�^
		m_pMorph	= nullptr;//���[�t�f�[�^
		m_pDisplay	= nullptr;//�\���g
	}
	~PMX_DATA()
	{
		//���_�f�[�^
		if (m_pVertex != nullptr)
		{
			delete[] m_pVertex;
			m_pVertex = nullptr;
		}
		//�ʂ̃f�[�^
		if (m_pFace != nullptr)
		{
			delete[] m_pFace;
			m_pFace = nullptr;
		}
		//�e�N�X�`���f�[�^
		if (m_pTex != nullptr)
		{
			delete[] m_pTex;
			m_pTex = nullptr;
		}
		//�}�e���A���f�[�^
		if (m_pMaterial != nullptr)
		{
			delete[] m_pMaterial;
			m_pMaterial = nullptr;
		}
		//�{�[���f�[�^
		if (m_pBone != nullptr)
		{
			delete[] m_pBone;
			m_pBone = nullptr;
		}
		//���[�t�f�[�^
		if (m_pMorph != nullptr)
		{
			delete[] m_pMorph;
			m_pMorph = nullptr;
		}
		//�\���g
		if (m_pDisplay != nullptr)
		{
			delete[] m_pDisplay;
			m_pDisplay = nullptr;
		}
	}

	int		m_VerNum;		//���_��
	int		m_FaceNum;		//�ʂ̐�
	int		m_TexNum;		//�e�N�X�`����
	int		m_MaterialNum;	//�}�e���A����
	int		m_BoneNum;		//�{�[����
	int		m_MorphNum;		//���[�t��
	int		m_DisplayNum;	//�\���g��

	PMX_HEADER		m_Head;			//�w�b�_�[
	PMX_MODEL_INFO	m_ModelInfo;	//���f���f�[�^
	PMX_VERTEX*		m_pVertex;		//���_�f�[�^
	PMX_FACE*		m_pFace;		//�ʂ̃f�[�^
	PMX_TEXTURE*	m_pTex;			//�e�N�X�`���f�[�^
	PMX_MATERIAL*	m_pMaterial;	//�}�e���A���f�[�^
	PMX_BONE*		m_pBone;		//�{�[���f�[�^
	PMX_MORPH*		m_pMorph;		//���[�t�f�[�^	
	PMX_DISPLAY* m_pDisplay;		//�\���g�f�[�^
};

//PMX�ǂݍ���
class CPmxLoader :public CSingleton<CPmxLoader>
{
private:
	//�V���O���g��
	friend CSingleton<CPmxLoader>;
	CPmxLoader() {};
	~CPmxLoader() {};
public:

	//�ǂݍ���
	bool Load(const char* FileName, PMX_DATA* pPmxData);
	//�����o��
	bool Write(const char* FileName, PMX_DATA* pPmxData);
private:
	//�w�b�_���ǂ݂���
	void HeadLoad(FILE* fp, PMX_DATA* pPmxData);

	//���f�����ǂݍ���
	void ModelInfoLoad(FILE* fp, PMX_DATA* pPmxData);

	//���_�ǂݍ���
	bool VertexLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//�ʓǂݍ���
	bool FaceLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//�e�N�X�`���ǂݍ���
	bool TextureLoad(FILE* fp, PMX_DATA* pPmxData);

	//�}�e���A���ǂݍ���
	bool MaterialLoad(FILE* fp, PMX_DATA* pPmxData);

	//�{�[���ǂݍ���
	bool BoneLoad(FILE* fp, PMX_DATA* pPmxData);

	//���[�t�ǂݍ���
	bool MorphLoad(FILE* fp, PMX_DATA* pPmxData);

	//�\���g�ǂݍ���
	bool DisplayFrameLoad(FILE* fp, PMX_DATA* pPmxData);
};
