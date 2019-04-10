#include "myAudio.h"


DWORD bufSize = 1 << 10;
myAudio *myAudio::ptrthis = NULL;
FILE* myAudio::fp = NULL;

char* filename = DefaultPhoneInfo.filename;
char* mp3FileName = DefaultPhoneInfo.mp3FileName;
char* wavFileName = DefaultPhoneInfo.wavFileName;

//extern char filename[30];
//extern char mp3FileName[30];
//extern char wavFileName[30];
//const char *mp3FileName = "myvioce.mp3";
//const char *wavFileName = "myvioce.wav";

//extern std::string CurrentPhoneNum;

#define INBUFSIZE 8192
#define MP3BUFSIZE 8192

baseMyAudio::baseMyAudio(int _buffNum)
	:buffNum(_buffNum)
{
	/*mywaveform.wFormatTag = WAVE_FORMAT_PCM;
	mywaveform.nChannels = 1;
	mywaveform.nSamplesPerSec = 8000;
	mywaveform.wBitsPerSample = 16;
	mywaveform.cbSize = 0;
	mywaveform.nBlockAlign = (mywaveform.wBitsPerSample * mywaveform.nChannels) >> 3;
	mywaveform.nAvgBytesPerSec = mywaveform.nBlockAlign * mywaveform.nSamplesPerSec;

	pBuff = new BYTE*[buffNum];
	for (int i=0;i<buffNum;++i)
	{
		pBuff[i] = new BYTE[bufSize];
	}


	wavehdr = new WAVEHDR*[buffNum];
	for (int i = 0;i < buffNum;++i)
	{
		wavehdr[i]= new WAVEHDR;
	}
	for (int i=0;i<buffNum;++i)
	{
		wavehdr[i]->lpData = (LPSTR)(pBuff[i]);
		wavehdr[i]->dwBufferLength = bufSize;
		wavehdr[i]->dwBytesRecorded = 0;
		wavehdr[i]->dwUser = 0;
		wavehdr[i]->dwFlags = 0;
		wavehdr[i]->dwLoops = 1;
		wavehdr[i]->lpNext = NULL;
		wavehdr[i]->reserved = 0;
	}*/
	/*
	pBuff1 = new BYTE[bufSize];
	pBuff2 = new BYTE[bufSize];

	wavehdr1.lpData = (LPSTR)pBuff1;
	wavehdr1.dwBufferLength = bufSize;
	wavehdr1.dwBytesRecorded = 0;
	wavehdr1.dwUser = 0;
	wavehdr1.dwFlags = 0;
	wavehdr1.dwLoops = 1;
	wavehdr1.lpNext = NULL;
	wavehdr1.reserved = 0;
	
	wavehdr2.lpData = (LPSTR)pBuff2;
	wavehdr2.dwBufferLength = bufSize;
	wavehdr2.dwBytesRecorded = 0;
	wavehdr2.dwUser = 0;
	wavehdr2.dwFlags = 0;
	wavehdr2.dwLoops = 1;
	wavehdr2.lpNext = NULL;
	wavehdr2.reserved = 0;*/
	
	/*wavehdrout = new WAVEHDR*[buffNum];
	for (int i = 0;i < buffNum;++i)
	{
		wavehdrout[i] = new WAVEHDR;
	}
	for (int i = 0;i < buffNum;++i) {
		wavehdrout[i]->lpData = (LPSTR)pBuff[i];
		wavehdrout[i]->dwBufferLength = bufSize;
		wavehdrout[i]->dwBytesRecorded = 0;
		wavehdrout[i]->dwUser = 0;
		wavehdrout[i]->dwFlags = 0;
		wavehdrout[i]->dwLoops = 1;
		wavehdrout[i]->lpNext = NULL;
		wavehdrout[i]->reserved = 0;

	}*/
	/*wavehdr11.lpData = (LPSTR)pBuff1;
	wavehdr11.dwBufferLength = bufSize;
	wavehdr11.dwBytesRecorded = 0;
	wavehdr11.dwUser = 0;
	wavehdr11.dwFlags = 0;
	wavehdr11.dwLoops = 1;
	wavehdr11.lpNext = NULL;
	wavehdr11.reserved = 0;

	wavehdr12.lpData = (LPSTR)pBuff2;
	wavehdr12.dwBufferLength = bufSize;
	wavehdr12.dwBytesRecorded = 0;
	wavehdr12.dwUser = 0;
	wavehdr12.dwFlags = 0;
	wavehdr12.dwLoops = 1;
	wavehdr12.lpNext = NULL;
	wavehdr12.reserved = 0;*/

}

baseMyAudio::~baseMyAudio()
{
	
}

myAudio::~myAudio()
{

}
void CALLBACK myAudio::waveInBack(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	LPWAVEHDR pwhd = (PWAVEHDR)dwParam1;
	switch (uMsg)
	{
	case WIM_OPEN:
		cout << "Begin recording." << endl;
		break;
	case WIM_DATA:
		//cout << "recive" << endl;
		
		//fseek(fp, 0, SEEK_SET);
		
		fwrite(pwhd->lpData,1, pwhd->dwBufferLength,  fp);
		waveInUnprepareHeader(hwi, pwhd, sizeof(WAVEHDR));
		waveInPrepareHeader(hwi, pwhd, sizeof(WAVEHDR));
		waveInAddBuffer(hwi, pwhd, sizeof(WAVEHDR));
		break;
	case WIM_CLOSE:
		cout << "close wim" << endl;
		waveInReset(hwi);
		waveInUnprepareHeader(hwi, pwhd, sizeof(WAVEHDR));
		cout << " End Recording" << endl;
		//waveInClose(hwi);
		break;
	default:
		cout << " wavein back function error" << endl;
		break;
	}
}

void CALLBACK myAudio::waveOutBack(HWAVEOUT hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	LPWAVEHDR pwhr = (PWAVEHDR)dwParam1;
	switch (uMsg)
	{
	case WOM_CLOSE:
		cout << "waveout close" << endl;
		waveOutReset(hwi);
		waveOutUnprepareHeader(hwi, pwhr, sizeof(WAVEHDR));
		waveOutClose(hwi);
		
		fclose(fp);
		break;
	case WOM_DONE:
		//cout << "play " << endl;
		fread(pwhr->lpData, 1,pwhr->dwBufferLength,fp);
		waveOutPrepareHeader(hwi, pwhr, sizeof(WAVEHDR));
		waveOutWrite(hwi, pwhr, sizeof(WAVEHDR));
		break;
	case WOM_OPEN:
		//cout << "BEGIN PLAYING" << endl;
		break;
	default:
		cout << " waveout back function error" << endl;
		break;
	}
}
bool myAudio::recordAudio()
{
	/*mywaveform.wFormatTag = WAVE_FORMAT_PCM;
	mywaveform.nChannels = 1;
	mywaveform.nSamplesPerSec = 8000;
	mywaveform.wBitsPerSample = 16;
	mywaveform.cbSize = 0;
	mywaveform.nBlockAlign = (mywaveform.wBitsPerSample * mywaveform.nChannels) >> 3;
	mywaveform.nAvgBytesPerSec = mywaveform.nBlockAlign * mywaveform.nSamplesPerSec;



	pBuff = new BYTE*[buffNum];
	for (int i = 0;i < buffNum;++i)
	{
		pBuff[i] = new BYTE[bufSize];
	}*/
	makeSettings();

//	wavehdr = new WAVEHDR*[buffNum];
	for (int i = 0;i < buffNum;++i)
	{
		wavehdr[i] = new WAVEHDR;
	}
	for (int i = 0;i < buffNum;++i)
	{
		wavehdr[i]->lpData = (LPSTR)(pBuff[i]);
		wavehdr[i]->dwBufferLength = bufSize;
		wavehdr[i]->dwBytesRecorded = 0;
		wavehdr[i]->dwUser = 0;
		wavehdr[i]->dwFlags = 0;
		wavehdr[i]->dwLoops = 1;
		wavehdr[i]->lpNext = NULL;
		wavehdr[i]->reserved = 0;
	}
//	strcpy(filename, (soft_GetNowTime("%Y%m%d%H%M%S") + "-" + CurrentPhoneNum).c_str());
	sprintf(mp3FileName, "%s%s", filename, ".mp3");
	fp = fopen(filename, "wb+");

	MMRESULT opnRet = waveInOpen(&waveInputDev, WAVE_MAPPER, &mywaveform, (DWORD_PTR)waveInBack, 0, CALLBACK_FUNCTION);
	if (opnRet != MMSYSERR_NOERROR){
		cout << "waveInOpen error" << endl;
		return false;
	}
	for (int i=0;i<buffNum;++i){
		MMRESULT prepareRet1 = waveInPrepareHeader(waveInputDev, wavehdr[i], sizeof(WAVEHDR));
		if (prepareRet1 != MMSYSERR_NOERROR) {
			cout << "waveInPrepareHeader1 error" << endl;
			return false;
		}
	}
	/*MMRESULT prepareRet1 = waveInPrepareHeader(waveInputDev, &wavehdr1, sizeof(WAVEHDR));
	if (prepareRet1 != MMSYSERR_NOERROR) {
		cout << "waveInPrepareHeader1 error" << endl;
		return false;
	}


	MMRESULT prepareRet2 = waveInPrepareHeader(waveInputDev, &wavehdr2, sizeof(WAVEHDR));
	if (prepareRet2 != MMSYSERR_NOERROR) {
		cout << "waveInPrepareHeader2 error" << endl;
		return false;
	}*/
	for (int i = 0;i < buffNum;++i) {
		MMRESULT addBufRet1 = waveInAddBuffer(waveInputDev, wavehdr[i], sizeof(WAVEHDR));
		if (addBufRet1 != MMSYSERR_NOERROR) {
			cout << "waveInAddBuffer1 error" << endl;
			return false;
		}
	}
	/*MMRESULT addBufRet1= waveInAddBuffer(waveInputDev, &wavehdr1, sizeof(WAVEHDR));
	if (addBufRet1 != MMSYSERR_NOERROR) {
		cout << "waveInAddBuffer1 error" << endl;
		return false;
	}

	MMRESULT addBufRet2 = waveInAddBuffer(waveInputDev, &wavehdr2, sizeof(WAVEHDR));
	if (addBufRet2 != MMSYSERR_NOERROR) {
		cout << "waveInAddBuffer2 error" << endl;
		return false;
	}*/

	MMRESULT startResult = waveInStart(waveInputDev);
	if (startResult != MMSYSERR_NOERROR) {
		cout << "waveInAddBuffer2 error" << endl;
		return false;
	}
//	free(wavehdr);
	return true;
}
bool myAudio::stopRecord()
{

	waveInClose(waveInputDev);
	waveInStop(waveInputDev);
	if (fp != NULL) {
		fclose(fp);
	}
	
	return true;
}
bool myAudio::stopPlayer()
{
	
	waveOutPause(waveOutDev);
	if (fp != NULL){
		fclose(fp);
	}
	return true;
}
bool myAudio::playerAudio()
{
	/*mywaveform.wFormatTag = WAVE_FORMAT_PCM;
	mywaveform.nChannels = 1;
	mywaveform.nSamplesPerSec = 8000;
	mywaveform.wBitsPerSample = 16;
	mywaveform.cbSize = 0;
	mywaveform.nBlockAlign = (mywaveform.wBitsPerSample * mywaveform.nChannels) >> 3;
	mywaveform.nAvgBytesPerSec = mywaveform.nBlockAlign * mywaveform.nSamplesPerSec;



	pBuff = new BYTE*[buffNum];
	for (int i = 0;i < buffNum;++i)
	{
		pBuff[i] = new BYTE[bufSize];
	}*/
/*	makeSettings();
	fp = fopen(filename, "rb");

	wavehdrout = new WAVEHDR*[buffNum];
	for (int i = 0;i < buffNum;++i)
	{
		wavehdrout[i] = new WAVEHDR;
	}
	for (int i = 0;i < buffNum;++i) {
		wavehdrout[i]->lpData = (LPSTR)pBuff[i];
		wavehdrout[i]->dwBufferLength = bufSize;
		wavehdrout[i]->dwBytesRecorded = 0;
		wavehdrout[i]->dwUser = 0;
		wavehdrout[i]->dwFlags = 0;
		wavehdrout[i]->dwLoops = 1;
		wavehdrout[i]->lpNext = NULL;
		wavehdrout[i]->reserved = 0;

	}

	if (fp == NULL)
	{
		cout << "player open file error" << endl;
		return false;
	}

	waveOutOpen(&waveOutDev,WAVE_MAPPER, &mywaveform, (DWORD_PTR)waveOutBack, 0, CALLBACK_FUNCTION);

	for (int i=0;i<buffNum;++i){
		//fread(wavehdrout[i]->lpData, 1, wavehdrout[i]->dwBufferLength, fp);
		
		if (0 == (fread(wavehdrout[i]->lpData, 1, wavehdrout[i]->dwBufferLength, fp))) {
			cout << "fread error" << endl;
			return false;
		}
		waveOutPrepareHeader(waveOutDev, wavehdrout[i], sizeof(WAVEHDR));
	}


	for (int i = 0;i < buffNum;++i) {
		waveOutWrite(waveOutDev, wavehdrout[i], sizeof(WAVEHDR));
	}
	
	cout << "wave out success" << endl;

	delete[] wavehdrout;*/
	return true;
}
bool myAudio::releaseSource()
{
	for (int i = 0;i < buffNum;++i) {
		if (wavehdr[i])
		{
			delete[]wavehdr[i];
			wavehdr[i] = NULL;
		}
	}
	delete[]wavehdr;
	wavehdr = NULL;

	for (int i = 0;i < buffNum;++i) {
		if (wavehdrout[i])
		{
			delete[]wavehdrout[i];
			wavehdrout[i] = NULL;
		}
	}
	delete[]wavehdrout;
	wavehdrout = NULL;

	for (int i = 0;i < buffNum;++i) {
		if (pBuff[i])
		{
			delete[]pBuff[i];
			pBuff[i] = NULL;
		}
	}
	delete[]pBuff;
	pBuff = NULL;
	cout << "release success" << endl;
	return true;
}


bool  myAudio::makeSettings()
{
	mywaveform.wFormatTag = WAVE_FORMAT_PCM;
	mywaveform.nChannels = 2;
	mywaveform.nSamplesPerSec = 44100;
	mywaveform.wBitsPerSample = 16;
	mywaveform.cbSize = 0;
	mywaveform.nBlockAlign = (mywaveform.wBitsPerSample * mywaveform.nChannels) /8;
	mywaveform.nAvgBytesPerSec = mywaveform.nBlockAlign * mywaveform.nSamplesPerSec;



	pBuff = new BYTE*[buffNum];
	for (int i = 0;i < buffNum;++i)
	{
		pBuff[i] = new BYTE[bufSize];
	}
	delete[] pBuff;
	return true;
}

bool myAudio::transfPcmToWav()
{
	FILE *wavFp;
	wavFp = fopen(wavFileName, "wb+");
	
	long headerSize = 38;
	long formatSize = 18;

	
	fwrite("RIFF",1,4,wavFp);
	FILE* file = NULL;
	file = fopen(filename, "rb");
	if (fp == NULL){
		cout << "pcm file open error" << endl;
		return false;
	}
	fseek(file, 0, SEEK_END);
	unsigned long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	unsigned int size = fileSize + headerSize;
	fwrite(&size, 1,sizeof(size), wavFp);
	fwrite("WAVE", 1, 4, wavFp);
	fwrite("fmt ", 1, 4, wavFp);
	fwrite(&formatSize,1,sizeof(formatSize), wavFp);
	fwrite(&mywaveform.wFormatTag, 1, sizeof(mywaveform.wFormatTag), wavFp);
	fwrite(&mywaveform.nChannels, 1, sizeof(mywaveform.nChannels), wavFp);
	fwrite(&mywaveform.nSamplesPerSec, 1, sizeof(mywaveform.nSamplesPerSec), wavFp);
	fwrite(&mywaveform.nAvgBytesPerSec, 1, sizeof(mywaveform.nAvgBytesPerSec), wavFp);
	fwrite(&mywaveform.nBlockAlign, 1, sizeof(mywaveform.nBlockAlign), wavFp);
	fwrite(&mywaveform.wBitsPerSample, 1, sizeof(mywaveform.wBitsPerSample), wavFp);
	fwrite(&mywaveform.cbSize, 1, sizeof(mywaveform.cbSize), wavFp);
	fwrite("data", 1, 4, wavFp);
	fwrite(&mywaveform.cbSize, 1, sizeof(mywaveform.cbSize), wavFp);
	fwrite(&fileSize, 1, sizeof(&fileSize), wavFp);
	

	long readNum = 0;
	int cacheSize = fileSize;
	char *voiceData = new char[cacheSize];
	while (readNum = fread(voiceData,sizeof(char),cacheSize,file))
	{
		fwrite(voiceData, sizeof(char), readNum, wavFp);
	}
	delete []voiceData;
	fclose(file);
	fclose(wavFp);
	return true;
}
bool myAudio::transfPcmToMp3()
{
	bool status = true;
	lame_global_flags *lame;
	int retCode;
	FILE* infp;
	FILE* outfp;
	short* inbuff;
	int inputSamples;
	unsigned char* mp3buff;
	int mp3bytes;



	infp = fopen(filename, "rb");
	outfp = fopen(mp3FileName, "wb");

	inbuff = new short[INBUFSIZE * 2];
	mp3buff = new BYTE[MP3BUFSIZE];

	//初始化编码器
	lame = lame_init();
	if (lame == NULL)
	{
		cout << "lame_init failed" << endl;
		status = false;
		lame_close(lame);
	}
	//设置音频的采样率
	lame_set_in_samplerate(lame, 44100);

	//设置lame编码器的声道
	lame_set_num_channels(lame, 2);

	//vbr设置采样压缩格式
	lame_set_VBR(lame, vbr_default);

	//初始化lame编码器
	retCode = lame_init_params(lame);
	if (retCode < 0) {
		cout << "lame_init_params failed" << retCode << endl;
		status = false;
	}

	do {
		inputSamples = fread(inbuff, 2 * sizeof(short int), INBUFSIZE, infp);
		cout << "inputSamples is " << inputSamples << endl;

		if (inputSamples == 0) {
			mp3bytes = lame_encode_flush(lame, mp3buff, MP3BUFSIZE);
		}
		else {
			mp3bytes = lame_encode_buffer_interleaved(lame, inbuff, inputSamples, mp3buff, MP3BUFSIZE);
		}

		fwrite(mp3buff, 1, mp3bytes, outfp);
		/*mp3bytes = lame_encode_buffer_interleaved(lgf, inbuff, inputSamples / 2, mp3buff, MP3BUFSIZE);

		if (mp3bytes < 0) {
		cout << "lame_encode_buffer_interleaved failed" << mp3bytes << endl;
		status = false;
		goto free_buff;
		}
		else if (mp3bytes > 0) {
		fwrite(mp3buff, 1, mp3bytes, outfp);
		}*/
	} while (inputSamples != 0);


	mp3bytes = lame_encode_flush(lame, mp3buff, sizeof(mp3buff));
	if (mp3bytes > 0)
	{
		cout << "mp3 bytes is " << mp3bytes << endl;
		fwrite(mp3buff, 1, mp3bytes, outfp);
	}
//	free(mp3buff);
//	free(inbuff);
	delete[] mp3buff;
	delete[] inbuff;

	fclose(infp);
	fclose(outfp);

	lame_close(lame);

	return status;
}
//bool myAudio::transfPcmToMp3()
//{
//	bool status = true;
//	lame_global_flags *lame;
//	int retCode;
//	FILE* infp;
//	FILE* outfp;
//	short* inbuff;
//	int inputSamples;
//	unsigned char* mp3buff;
//	int mp3bytes;
//	
//	
//
//	infp = fopen(filename, "rb");
//	outfp = fopen(mp3FileName, "wb");
//
//	inbuff = new short[INBUFSIZE * 2];
//	mp3buff = new BYTE[MP3BUFSIZE];
//
//	//初始化编码器
//	lame = lame_init();
//	if (lame == NULL)
//	{
//		cout << "lame_init failed" << endl;
//		status = false;
//		lame_close(lame);
//	}
//	//设置音频的采样率
//	lame_set_in_samplerate(lame, 22050);
//
//	//设置lame编码器的声道
//	lame_set_num_channels(lame, 1);
//
//	//vbr设置采样压缩格式
//	lame_set_VBR(lame, vbr_default);
//
//	//初始化lame编码器
//	retCode = lame_init_params(lame);
//	if (retCode < 0) {
//		cout << "lame_init_params failed" << retCode << endl;
//		status = false;
//	}
//
//	do {
//		inputSamples = fread(inbuff, 2*sizeof(short int), INBUFSIZE, infp);
//		cout << "inputSamples is " << inputSamples << endl;
//
//		if (inputSamples == 0){
//			mp3bytes = lame_encode_flush(lame, mp3buff, MP3BUFSIZE);
//		}
//		else {
//			mp3bytes = lame_encode_buffer_interleaved(lame, inbuff, inputSamples, mp3buff, MP3BUFSIZE);
//		}
//
//		fwrite(mp3buff, 1, mp3bytes, outfp);
//		/*mp3bytes = lame_encode_buffer_interleaved(lgf, inbuff, inputSamples / 2, mp3buff, MP3BUFSIZE);
//
//		if (mp3bytes < 0) {
//			cout << "lame_encode_buffer_interleaved failed" << mp3bytes << endl;
//			status = false;
//			goto free_buff;
//		}
//		else if (mp3bytes > 0) {
//			fwrite(mp3buff, 1, mp3bytes, outfp);
//		}*/
//	} while (inputSamples != 0);
//
//
//	mp3bytes = lame_encode_flush(lame, mp3buff, sizeof(mp3buff));
//	if (mp3bytes > 0)
//	{
//		cout << "mp3 bytes is " << mp3bytes << endl;
//		fwrite(mp3buff, 1, mp3bytes, outfp);
//	}
//	free(mp3buff);
//	free(inbuff);
//
//	fclose(infp);
//	fclose(outfp);
//
//	lame_close(lame);
//
//	return status;
//}
