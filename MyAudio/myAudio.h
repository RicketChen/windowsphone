#ifndef MYDUDIO_H
#define MYDUDIO_H

#include <iostream>
#include <Windows.h>
#include "mmsystem.h"
#include <fstream>
#include "libmp3lame/lame.h"
#include "soft_function.h"
#include "stdafx.h"
#pragma comment(lib,"Winmm.lib")
using namespace std;

extern PhoneInfo_t DefaultPhoneInfo;

class baseMyAudio 
{
public:
	baseMyAudio(int _buffNum =2 );
	~baseMyAudio();
	virtual bool recordAudio() = 0;
	virtual bool playerAudio() = 0;
	virtual bool stopRecord() = 0;
	virtual bool stopPlayer() = 0;
	virtual bool releaseSource() = 0;
	virtual bool makeSettings() = 0;
	virtual bool transfPcmToWav() = 0;
	virtual bool transfPcmToMp3() = 0;
protected:
	HWAVEIN waveInputDev;
	HWAVEOUT waveOutDev;

	WAVEFORMATEX mywaveform;
	//PBYTE pBuff1, pBuff2;
	BYTE **pBuff;
	/*WAVEHDR wavehdr1, wavehdr2;*/
	WAVEHDR **wavehdr;

	/*WAVEHDR wavehdr11, wavehdr12;*/
	WAVEHDR **wavehdrout;

	int buffNum;
};
class myAudio : public baseMyAudio
{
public:
	myAudio(int _numofbuff) : baseMyAudio(_numofbuff )
	{
		ptrthis = this;
	}
	~myAudio();
	bool recordAudio();
	virtual bool playerAudio();
	bool stopRecord();
	bool stopPlayer();
	bool releaseSource();
	bool makeSettings();
	bool transfPcmToWav();
	bool transfPcmToMp3();

private:
	static void CALLBACK waveInBack(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
	static void CALLBACK waveOutBack(HWAVEOUT hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
	static myAudio* ptrthis;
	static FILE *fp;

};
#endif // 