#include "txttowav.h"
#include "OSS_MAIN.h"

using namespace std;

extern PhoneInfo_t DefaultPhoneInfo;

unsigned char WavHeadData[44] = {
	0x52, 0x49, 0x46, 0x46,							//00H 	4 	char 		"RIFF"
	0x00, 0x00, 0x00, 0x00,							//04H 	4 	long 		int 文件总长-8 long int size0=文总长-8
	0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,	//08H 	8 	char 		"WAVEfmt "
	0x10, 0x00, 0x00, 0x00,							//10H 	4 	long 		int 10 00 00 00H(PCM)
	0x01, 0x00,										//14H 	2 	int 		01 00H int fmttag=0x01
	0x01, 0x00,            							//16H 	2 	int 		int channel=1或2
	0x44,0xac,0x00,0x00,  	// samplespersec 16000(0x80,0x3E,0x00,0x00) 8000(0x40, 0x1F, 0x00, 0x00)
	0x10,0xB1,0x02,0x00,							//1CH 	4 	long 		int 每秒播放字节数 long int bytepersec
	0x04,0x00,										//20H 	2 	int 		采样一次占字节数 int blockalign=声道数*量化数/8
	0x10,0x00,										//22H 	2 	int 		量化数 int bitpersamples=8或16
	0x64,0x61,0x74,0x61,							//24H 	4 	char 		"data"
	0x00,0x00,0x00,0x00								//28H 	4 	long 		int 采样数据字节数 long int size2=文长-44
};
void wavset()
{
	unsigned short	channels;
	unsigned int	longSampleRate;
	unsigned int	byteRate;
	unsigned short	bitsPerSample;
	bitsPerSample = 16;			//采样位数
	channels = 1;
	longSampleRate = 8000;		//采样频率
	byteRate = longSampleRate * (bitsPerSample / 8) * channels;

	WavHeadData[0] = 'R'; // RIFF
	WavHeadData[1] = 'I';
	WavHeadData[2] = 'F';
	WavHeadData[3] = 'F';
	//文件长度  4字节文件长度，这个长度不包括"RIFF"标志(4字节)和文件长度本身所占字节(4字节),即该长度等于整个文件长度 - 8
	WavHeadData[4] = 0;
	WavHeadData[5] = 0;
	WavHeadData[6] = 0;
	WavHeadData[7] = 0;
	//fcc type：4字节 "WAVE" 类型块标识, 大写
	WavHeadData[8] = 'W';
	WavHeadData[9] = 'A';
	WavHeadData[10] = 'V';
	WavHeadData[11] = 'E';
	//FMT Chunk   4字节 表示"fmt" chunk的开始,此块中包括文件内部格式信息，小写, 最后一个字符是空格
	WavHeadData[12] = 'f'; // 'fmt '
	WavHeadData[13] = 'm';
	WavHeadData[14] = 't';
	WavHeadData[15] = ' ';//过渡字节
	//数据大小  4字节，文件内部格式信息数据的大小，过滤字节（一般为00000010H）
	WavHeadData[16] = 16;
	WavHeadData[17] = 0;
	WavHeadData[18] = 0;
	WavHeadData[19] = 0;
	//编码方式 10H为PCM编码格式   FormatTag：2字节，音频数据的编码方式，1：表示是PCM 编码
	WavHeadData[20] = 1; // format = 1
	WavHeadData[21] = 0;
	//通道数  Channels：2字节，声道数，单声道为1，双声道为2
	WavHeadData[22] = (byte)channels;
	WavHeadData[23] = 0;
	//采样率，每个通道的播放速度
	WavHeadData[24] = (byte)(longSampleRate & 0xff);
	WavHeadData[25] = (byte)((longSampleRate >> 8) & 0xff);
	WavHeadData[26] = (byte)((longSampleRate >> 16) & 0xff);
	WavHeadData[27] = (byte)((longSampleRate >> 24) & 0xff);
	//音频数据传送速率,采样率*通道数*采样深度/8
	//4字节，音频数据传送速率, 单位是字节。其值为采样率×每次采样大小。播放软件利用此值可以估计缓冲区的大小
	//byteRate = sampleRate * (bitsPerSample / 8) * channels
	WavHeadData[28] = (byte)(byteRate & 0xff);
	WavHeadData[29] = (byte)((byteRate >> 8) & 0xff);
	WavHeadData[30] = (byte)((byteRate >> 16) & 0xff);
	WavHeadData[31] = (byte)((byteRate >> 24) & 0xff);
	// 确定系统一次要处理多少个这样字节的数据，确定缓冲区，通道数*采样位数
	WavHeadData[32] = (byte)(bitsPerSample * channels / 8);
	WavHeadData[33] = 0;
	//每个样本的数据位数
	//2字节，每个声道的采样精度; 譬如 16bit 在这里的值就是16。如果有多个声道，则每个声道的采样精度大小都一样的；
	WavHeadData[34] = byte(bitsPerSample);
	WavHeadData[35] = 0;
	//Data chunk
	//ckid：4字节，数据标志符（data），表示 "data" chunk的开始。此块中包含音频数据，小写；
	WavHeadData[36] = 'd';
	WavHeadData[37] = 'a';
	WavHeadData[38] = 't';
	WavHeadData[39] = 'a';
	//音频数据的长度，4字节，audioDataLen = totalDataLen - 36 = fileLenIncludeWavHeadData - 44
	WavHeadData[40] = 0;
	WavHeadData[41] = 0;
	WavHeadData[42] = 0;
	WavHeadData[43] = 0;

}
int txt_to_wav()
{
/*	char* buff = (char*) malloc(1000);
	char* outbuff = (char*)malloc(4096);
	char* audiobuff = (char*)malloc(4096);*/

	char buff[1000] = { 0 };
	char outbuff[4096] = { 0 };
	char audiobuff[4096] = { 0 };
	int newframe = 0;
	long outlen = 0;
	bool trans = false;
	wavset();
	int wavlen = sizeof(WavHeadData);
/*	strcat(DefaultPhoneInfo.wavFileName, soft_GetNowTime("%Y%m%d%H%M%S").c_str());
	strcat(DefaultPhoneInfo.wavFileName, DefaultPhoneInfo.agentId);
	strcat(DefaultPhoneInfo.wavFileName, DefaultPhoneInfo.company);
	strcat(DefaultPhoneInfo.wavFileName, DefaultPhoneInfo.mobile);
	strcat(DefaultPhoneInfo.wavFileName, DefaultPhoneInfo.called);*/

//	cout << "call time is " << DefaultPhoneInfo.calltime << endl << "talk time is " << DefaultPhoneInfo.talltime;

	FILE* wavfd = fopen(DefaultPhoneInfo.wavFileName, "wb");
//	FILE* wavfd = fopen("test.wav", "wb");
	if (wavfd <= 0)
	{
		cout << "open wav fail" << endl;
		return 0;
	}
	fseek(wavfd, 0, SEEK_SET);

	fwrite(WavHeadData, sizeof(char), sizeof(WavHeadData), wavfd);
	
	ifstream input("InputData.txt");
//	input.open("InputData.txt",ios::binary|ios::trunc|ios::out|ios::in);
	if (!input.is_open())
	{
		cout << "open file fail" << endl;
		return 0;
	}
//	input.seekg(ios::beg);
	int len = 0;
	while (!input.eof())
	{
//		while(strstr(buff,"+NTLK: 2") == NULL)
			input.getline(buff, 1000);

		if (strstr(buff, "+NRECD: "))		//这里将NRECD: 960,1314意外的数据全部剔除，避免录音的问题
		{
			len = atoi(strstr(buff, "+NRECD: ") + strlen("+NRECD: "));
			newframe = 1;
			memset(buff, 0, sizeof(buff));
			continue;
		}
		else if (strstr(buff, "+NTALK: "))
		{
			memset(buff, 0, sizeof(buff));
			continue;
		}
		else if (strstr(buff, "+NTALK: "))
		{
			memset(buff, 0, sizeof(buff));
			continue;
		}
		else if (strstr(buff, "at+csq"))
		{
			while (strstr(buff, "OK") == NULL)
			{
				input.getline(buff, 1000);
			}
			continue;
		}
		if (newframe == 1)
		{
			if (strstr(buff, "=="))
			{
				len+=2;
			}
			else if (strstr(buff, "="))
			{
				len++;
			}
			if (strcmp(buff, "" ) == 0 || trans)
			{
				if (len / 3 * 4 != strlen(outbuff))
				{
					newframe = 0;
					memset(outbuff, 0, sizeof(outbuff));
					continue;
				}
				newframe = 0;
				base64_decodestate ds;
				base64_init_decodestate(&ds);
				int len = base64_decode_block(outbuff, strlen(outbuff), audiobuff, &ds);
				if (len < 0)
				{
					cout << "decode failed!" << endl;
				}
				outlen += len;
				fwrite(audiobuff, sizeof(char), len, wavfd);
				memset(outbuff, 0, sizeof(outbuff));
				memset(buff, 0, sizeof(buff));
				memset(audiobuff, 0, sizeof(audiobuff));
				continue;
			}
			strcat(outbuff, buff);
		}
	}
	WavHeadData[4] = (byte)((outlen + wavlen) & 0xff);
	WavHeadData[5] = (byte)(((outlen + wavlen) >> 8) & 0xff);
	WavHeadData[6] = (byte)(((outlen + wavlen) >> 16) & 0xff);
	WavHeadData[7] = (byte)(((outlen + wavlen) >> 24) & 0xff);
	WavHeadData[40] = (byte)(outlen & 0xff);
	WavHeadData[41] = (byte)((outlen >> 8) & 0xff);
	WavHeadData[42] = (byte)((outlen >> 16) & 0xff);
	WavHeadData[43] = (byte)((outlen >> 24) & 0xff);

	fseek(wavfd, 0, SEEK_SET);
	if (fwrite(WavHeadData, sizeof(char), sizeof(WavHeadData), wavfd) > 0)
	{
	//	cout << soft_GetNowTime().c_str() << "write wav file success" << endl;
	}

	input.close();
	input.sync();
//	fflush(wavfd);
	fclose(wavfd);
	upload(DefaultPhoneInfo.wavFileName);
/*	free(buff);
	free(outbuff);
	free(audiobuff);*/
	remove(DefaultPhoneInfo.wavFileName);
	return 0;
}