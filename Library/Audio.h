#pragma once

#include <stdio.h>
#include <Windows.h>
#include <XAudio2.h>

#include <vector>
using namespace std;

//RIFF�t�@�C���̃t�H�[�}�b�g��ǂݎ��
struct ChunkInfo
{
	ChunkInfo() { Size = 0, pData = nullptr, Loop = false, Start = false; }
	unsigned int Size;		//�`�����N���̃T�C�Y
	unsigned char* pData;	//�`�����N�f�[�^�̐擪�|�C���^
	bool Loop;
	bool Start;
};

class CAudio
{
public:
	void Init();
	void Update();
	void Delete();

	//���y�̓ǂݍ���
	void Load(int Id ,const char* Name,bool Loop);
	
	//�}�X�^�[�{�����[���̒���
	void MasterVolume(float t);

	//�X�^�[�g
	void StartMusic(int Id);

	//�X�g�b�v
	void StopMusic(int Id);
	
private:	
	//Wave�ǂݍ���
	unsigned char* LoadWave(ChunkInfo* pChunkInfo, WAVEFORMATEX* pWave, const char* Name, bool Loop);

	//char * ���@WORD�@�ɕϊ�
	WORD GetWord(unsigned char* pData);

	//char * ���@DWORD�@�ɕϊ�
	DWORD GetDWord(unsigned char* pData);

	//�`�����N��T���֐�
	ChunkInfo FindChunk(unsigned char* pData, char* pChunkName);

	IXAudio2* m_pXAudio2;						//XAudio2�I�u�W�F�N�g
	IXAudio2MasteringVoice* m_pMasteringVoice;	//�}�X�^�[�{�C�X
	IXAudio2SubmixVoice* m_pSubmixVoice;		//�T�u�~�N�X�C���^�[�t�F�[�X
	vector<ChunkInfo> m_ChunkData;				//�T�E���h���
	vector<unsigned char*> m_pResourceData;		//�T�E���h�t�@�C���̃|�C���^
	vector<IXAudio2SourceVoice*> m_pSourceVoice;//�T�E���h�{�C�X�̃C���^�[�t�F�[�X
};

extern CAudio g_Audio;