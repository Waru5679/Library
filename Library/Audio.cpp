#include "Audio.h"

CAudio g_Audio;

//初期化
void CAudio::Init()
{
	m_pXAudio2 = nullptr;
	m_pMasteringVoice = nullptr;
	m_pSubmixVoice = nullptr;

	//XAudioの作成
	unsigned int CreateFlag = 0;
	XAudio2Create(&m_pXAudio2, CreateFlag, XAUDIO2_DEFAULT_PROCESSOR);

	//マスターボイスの作成
	m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);

	if (m_pMasteringVoice != nullptr)
	{
		//ミックスボイス
		m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoice, 1, 44100, 0, 0, 0, 0);
	}

	MasterVolume(1.0f);
}

//音楽の読み込み
void CAudio::Load(int Id,const char* Name,bool Loop)
{
	WAVEFORMATEX WaveFormatEx;
	
	ChunkInfo Chunk;
	unsigned char* pResourceData = LoadWave(&Chunk, &WaveFormatEx, Name ,Loop);
	m_ChunkData.insert(m_ChunkData.begin() + Id, Chunk);

	m_pResourceData.insert(m_pResourceData.begin() + Id, pResourceData);

	//再生のためのインターフェースの作成
	if (m_pMasteringVoice != nullptr)
	{
		XAUDIO2_SEND_DESCRIPTOR Send = { 0,m_pSubmixVoice };
		XAUDIO2_VOICE_SENDS SendList = { 1,&Send };
	
		IXAudio2SourceVoice* pSourceVoice;
		m_pXAudio2->CreateSourceVoice(&pSourceVoice, &WaveFormatEx, 0U, 2.0f, NULL, &SendList);
		m_pSourceVoice.insert(m_pSourceVoice.begin() + Id, pSourceVoice);
	}
}

//再生
void CAudio::StartMusic(int Id)
{
	WAVEFORMATEX WaveFormatEx;

	if(m_pMasteringVoice!=nullptr)
	{
		//サウンドバッファをソースボイスに送信
		XAUDIO2_BUFFER SoundBuffer={ 0 };
		SoundBuffer.AudioBytes = m_ChunkData[Id].Size;
		SoundBuffer.pAudioData = (BYTE*)m_ChunkData[Id].pData;
		SoundBuffer.Flags = XAUDIO2_END_OF_STREAM;
		SoundBuffer.LoopCount = 0;
	
		XAUDIO2_SEND_DESCRIPTOR Send = { 0,m_pSubmixVoice };
		XAUDIO2_VOICE_SENDS SendList = { 1,&Send };
		m_pXAudio2->CreateSourceVoice(&m_pSourceVoice[Id], &WaveFormatEx, 0U, 2.0f, NULL, &SendList);

		//サウンドバッファセット
		HRESULT hr=m_pSourceVoice[Id]->SubmitSourceBuffer(&SoundBuffer);		

		//すでに再生中ではないかチェックする
		XAUDIO2_VOICE_STATE state;
		m_pSourceVoice[Id]->GetState(&state);

		//サウンドスタート
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

		 //再生が終わった時
		 if (state.BuffersQueued == 0)
		 {
			 //再生されていた時
			 if (m_ChunkData[i].Start == true)
			 {
				  //ループならもう一度再生
				 if (m_ChunkData[i].Loop == true)
				 {
					 StopMusic(i);	//停止
					 StartMusic(i);	//再生
				 }
				 else
				 {
					 StopMusic(i);	//停止
				 }
			 }
		 } 
	 }
}

//ストップ
void CAudio::StopMusic(int Id)
{
	m_pSourceVoice[Id]->Stop();					//音楽
	m_pSourceVoice[Id]->FlushSourceBuffers();	//サウンドボイスに保留中のバッファを破棄
	m_ChunkData[Id].Start = false;
}

//Wave読み込み
unsigned char* CAudio::LoadWave(ChunkInfo* pChunkInfo, WAVEFORMATEX* pWave, const char* Name,bool Loop)
{
	unsigned char* WaveData;

	//Waveファイルのオープン
	FILE* fp;
	fopen_s(&fp, Name, "rb");

	//ファイルサイズ取得
	unsigned int Size = 0;
	fseek(fp, 0, SEEK_END);
	Size = ftell(fp);
	WaveData = new unsigned char[Size];

	//ファイルデータをメモリに移す
	fseek(fp, 0, SEEK_SET);
	fread(WaveData, Size, 1, fp);
	fclose(fp);

	//RIFFファイルの解析
	//RIFFデータの先頭アドレスとRIFFデータのサイズを渡す
	
	ChunkInfo WaveChunk = FindChunk(WaveData, (char*)"fmt");
	unsigned char*p = WaveChunk.pData;

	//Wave情報の取得
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

	//波形データの先頭アドレスと波形データのサイズを渡す
	ChunkInfo info= FindChunk(WaveData, (char*)"data");
	pChunkInfo->Size = info.Size;
	pChunkInfo->pData = info.pData;
	pChunkInfo->Loop = Loop;
	
	return WaveData;
}

//マスターボリュームの調整
void CAudio::MasterVolume(float t)
{
	if(m_pMasteringVoice!=nullptr)
		m_pMasteringVoice->SetVolume(t, 0);
}

//char * を　WORD　に変換
WORD CAudio::GetWord(unsigned char* pData)
{
	WORD value = pData[0] | pData[1] << 8;
	return value;
}

//char * を　DWORD　に変換
DWORD CAudio::GetDWord(unsigned char* pData)
{
	WORD value = pData[0] | pData[1] << 8 | pData[2] << 16 | pData[3] << 24;
	return value;
}

//チャンクを探す関数
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

//破棄
void CAudio::Delete()
{	
	//ソースボイス破棄
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

	//リソース破棄
	m_pResourceData.erase(m_pResourceData.begin(), m_pResourceData.end());
	
	//サウンド情報破棄
	m_ChunkData.erase(m_ChunkData.begin(), m_ChunkData.end());
	
	//ミックスサウンドの破棄
	if (m_pSubmixVoice != nullptr)
	{
		m_pSubmixVoice->DestroyVoice();
	}

	//マスターボイスの破棄
	if (m_pMasteringVoice != nullptr)
	{
		m_pMasteringVoice->DestroyVoice();
	}

	//XAudio2インタフェースの破棄
	m_pXAudio2->Release();
}