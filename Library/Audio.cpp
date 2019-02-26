#include "Audio.h"

CAudio g_Audio;

//������
void CAudio::Init()
{
	m_pXAudio2 = nullptr;
	m_pMasteringVoice = nullptr;
	m_pSubmixVoice = nullptr;

	//XAudio�̍쐬
	unsigned int CreateFlag = 0;
	XAudio2Create(&m_pXAudio2, CreateFlag, XAUDIO2_DEFAULT_PROCESSOR);

	//�}�X�^�[�{�C�X�̍쐬
	m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);

	if (m_pMasteringVoice != nullptr)
	{
		//�~�b�N�X�{�C�X
		m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoice, 1, 44100, 0, 0, 0, 0);
	}

	MasterVolume(1.0f);
}

//���y�̓ǂݍ���
void CAudio::Load(int Id,const char* Name,bool Loop)
{
	WAVEFORMATEX WaveFormatEx;
	
	ChunkInfo Chunk;
	unsigned char* pResourceData = LoadWave(&Chunk, &WaveFormatEx, Name ,Loop);
	m_ChunkData.insert(m_ChunkData.begin() + Id, Chunk);

	m_pResourceData.insert(m_pResourceData.begin() + Id, pResourceData);

	//�Đ��̂��߂̃C���^�[�t�F�[�X�̍쐬
	if (m_pMasteringVoice != nullptr)
	{
		XAUDIO2_SEND_DESCRIPTOR Send = { 0,m_pSubmixVoice };
		XAUDIO2_VOICE_SENDS SendList = { 1,&Send };
	
		IXAudio2SourceVoice* pSourceVoice;
		m_pXAudio2->CreateSourceVoice(&pSourceVoice, &WaveFormatEx, 0U, 2.0f, NULL, &SendList);
		m_pSourceVoice.insert(m_pSourceVoice.begin() + Id, pSourceVoice);
	}
}

//�Đ�
void CAudio::StartMusic(int Id)
{
	WAVEFORMATEX WaveFormatEx;

	if(m_pMasteringVoice!=nullptr)
	{
		//�T�E���h�o�b�t�@���\�[�X�{�C�X�ɑ��M
		XAUDIO2_BUFFER SoundBuffer={ 0 };
		SoundBuffer.AudioBytes = m_ChunkData[Id].Size;
		SoundBuffer.pAudioData = (BYTE*)m_ChunkData[Id].pData;
		SoundBuffer.Flags = XAUDIO2_END_OF_STREAM;
		SoundBuffer.LoopCount = 0;
	
		XAUDIO2_SEND_DESCRIPTOR Send = { 0,m_pSubmixVoice };
		XAUDIO2_VOICE_SENDS SendList = { 1,&Send };
		m_pXAudio2->CreateSourceVoice(&m_pSourceVoice[Id], &WaveFormatEx, 0U, 2.0f, NULL, &SendList);

		//�T�E���h�o�b�t�@�Z�b�g
		HRESULT hr=m_pSourceVoice[Id]->SubmitSourceBuffer(&SoundBuffer);		

		//���łɍĐ����ł͂Ȃ����`�F�b�N����
		XAUDIO2_VOICE_STATE state;
		m_pSourceVoice[Id]->GetState(&state);

		//�T�E���h�X�^�[�g
		m_pSourceVoice[Id]->Start();

		m_ChunkData[Id].Start = true;
	}
}


void CAudio::Update()
{
	 XAUDIO2_VOICE_STATE state;
	 for (unsigned int i = 0; i < m_pSourceVoice.size(); i++)
	 {
		 m_pSourceVoice[i]->GetState(&state);

		 //�Đ����I�������
		 if (state.BuffersQueued == 0)
		 {
			 //�Đ�����Ă�����
			 if (m_ChunkData[i].Start == true)
			 {
				  //���[�v�Ȃ������x�Đ�
				 if (m_ChunkData[i].Loop == true)
				 {
					 StopMusic(i);	//��~
					 StartMusic(i);	//�Đ�
				 }
				 else
				 {
					 StopMusic(i);	//��~
				 }
			 }
		 } 
	 }
}

//�X�g�b�v
void CAudio::StopMusic(int Id)
{
	m_pSourceVoice[Id]->Stop();					//���y
	m_pSourceVoice[Id]->FlushSourceBuffers();	//�T�E���h�{�C�X�ɕۗ����̃o�b�t�@��j��
	m_ChunkData[Id].Start = false;
}

//Wave�ǂݍ���
unsigned char* CAudio::LoadWave(ChunkInfo* pChunkInfo, WAVEFORMATEX* pWave, const char* Name,bool Loop)
{
	unsigned char* WaveData;

	//Wave�t�@�C���̃I�[�v��
	FILE* fp;
	fopen_s(&fp, Name, "rb");

	//�t�@�C���T�C�Y�擾
	unsigned int Size = 0;
	fseek(fp, 0, SEEK_END);
	Size = ftell(fp);
	WaveData = new unsigned char[Size];

	//�t�@�C���f�[�^���������Ɉڂ�
	fseek(fp, 0, SEEK_SET);
	fread(WaveData, Size, 1, fp);
	fclose(fp);

	//RIFF�t�@�C���̉��
	//RIFF�f�[�^�̐擪�A�h���X��RIFF�f�[�^�̃T�C�Y��n��
	
	ChunkInfo WaveChunk = FindChunk(WaveData, (char*)"fmt");
	unsigned char*p = WaveChunk.pData;

	//Wave���̎擾
	pWave->wFormatTag = GetWord(p);
	p += sizeof(WORD);

	pWave->nChannels = GetWord(p);
	p += sizeof(WORD);

	pWave->nSamplesPerSec = GetDWord(p);
	p += sizeof(DWORD);

	pWave->nAvgBytesPerSec = GetDWord(p);
	p += sizeof(DWORD);

	pWave->nBlockAlign = GetWord(p);
	p += sizeof(WORD);

	pWave->wBitsPerSample = GetWord(p);
	p += sizeof(WORD);

	pWave->cbSize = GetWord(p);
	p += sizeof(WORD);

	//�g�`�f�[�^�̐擪�A�h���X�Ɣg�`�f�[�^�̃T�C�Y��n��
	ChunkInfo info= FindChunk(WaveData, (char*)"data");
	pChunkInfo->Size = info.Size;
	pChunkInfo->pData = info.pData;
	pChunkInfo->Loop = Loop;
	
	return WaveData;
}

//�}�X�^�[�{�����[���̒���
void CAudio::MasterVolume(float t)
{
	if(m_pMasteringVoice!=nullptr)
		m_pMasteringVoice->SetVolume(t, 0);
}

//char * ���@WORD�@�ɕϊ�
WORD CAudio::GetWord(unsigned char* pData)
{
	WORD value = pData[0] | pData[1] << 8;
	return value;
}

//char * ���@DWORD�@�ɕϊ�
DWORD CAudio::GetDWord(unsigned char* pData)
{
	WORD value = pData[0] | pData[1] << 8 | pData[2] << 16 | pData[3] << 24;
	return value;
}

//�`�����N��T���֐�
ChunkInfo CAudio::FindChunk(unsigned char* pData, char* pChunkName)
{
	unsigned CHUNKNAME_LENGTH = strlen(pChunkName);

	while (1)
	{
		bool bFind = true;
		for (unsigned int i = 0; i < CHUNKNAME_LENGTH; ++i)
		{
			if (pData[i] != pChunkName[i])
			{
				bFind = false;
				break;
			}
		}
		if (bFind == true)
		{
			ChunkInfo Info;
			Info.Size = pData[4 + 0] | pData[4 + 1] << 8 | pData[4 + 2] << 16 | pData[4 + 3] << 24;
			Info.pData = (unsigned char*)(pData + 8);
			return Info;
		}
		pData++;
	}
	return ChunkInfo();
}

//�j��
void CAudio::Delete()
{	
	//�\�[�X�{�C�X�j��
	for (unsigned int i = 0; i < m_pSourceVoice.size(); i++)
	{
		if (m_pSourceVoice[i] != nullptr)
		{
			m_pSourceVoice[i]->Stop();
			m_pSourceVoice[i]->FlushSourceBuffers();
			m_pSourceVoice[i]->DestroyVoice();
		}
	}
	m_pSourceVoice.erase(m_pSourceVoice.begin(), m_pSourceVoice.end());

	//���\�[�X�j��
	m_pResourceData.erase(m_pResourceData.begin(), m_pResourceData.end());
	
	//�T�E���h���j��
	m_ChunkData.erase(m_ChunkData.begin(), m_ChunkData.end());
	
	//�~�b�N�X�T�E���h�̔j��
	if (m_pSubmixVoice != nullptr)
	{
		m_pSubmixVoice->DestroyVoice();
	}

	//�}�X�^�[�{�C�X�̔j��
	if (m_pMasteringVoice != nullptr)
	{
		m_pMasteringVoice->DestroyVoice();
	}

	//XAudio2�C���^�t�F�[�X�̔j��
	m_pXAudio2->Release();
}