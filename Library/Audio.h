#pragma once

#include <stdio.h>
#include <Windows.h>
#include <XAudio2.h>

#include <vector>
using namespace std;

//RIFFファイルのフォーマットを読み取る
struct ChunkInfo
{
	ChunkInfo() { Size = 0, pData = nullptr, Loop = false, Start = false; }
	unsigned int Size;		//チャンク部のサイズ
	unsigned char* pData;	//チャンクデータの先頭ポインタ
	bool Loop;
	bool Start;
};

class CAudio
{
public:
	void Init();
	void Update();
	void Delete();

	//音楽の読み込み
	void Load(int Id ,const char* Name,bool Loop);
	
	//マスターボリュームの調整
	void MasterVolume(float t);

	//スタート
	void StartMusic(int Id);

	//ストップ
	void StopMusic(int Id);
	
private:	
	//Wave読み込み
	unsigned char* LoadWave(ChunkInfo* pChunkInfo, WAVEFORMATEX* pWave, const char* Name, bool Loop);

	//char * を　WORD　に変換
	WORD GetWord(unsigned char* pData);

	//char * を　DWORD　に変換
	DWORD GetDWord(unsigned char* pData);

	//チャンクを探す関数
	ChunkInfo FindChunk(unsigned char* pData, char* pChunkName);

	IXAudio2* m_pXAudio2;						//XAudio2オブジェクト
	IXAudio2MasteringVoice* m_pMasteringVoice;	//マスターボイス
	IXAudio2SubmixVoice* m_pSubmixVoice;		//サブミクスインターフェース
	vector<ChunkInfo> m_ChunkData;				//サウンド情報
	vector<unsigned char*> m_pResourceData;		//サウンドファイルのポインタ
	vector<IXAudio2SourceVoice*> m_pSourceVoice;//サウンドボイスのインターフェース
};

extern CAudio g_Audio;