#include "txttowav.h"
#include "OSS_MAIN.h"

using namespace std;

extern PhoneInfo_t DefaultPhoneInfo;

unsigned char WavHeadData[44] = {
	0x52, 0x49, 0x46, 0x46,							//00H 	4 	char 		"RIFF"
	0x00, 0x00, 0x00, 0x00,							//04H 	4 	long 		int �ļ��ܳ�-8 long int size0=���ܳ�-8
	0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,	//08H 	8 	char 		"WAVEfmt "
	0x10, 0x00, 0x00, 0x00,							//10H 	4 	long 		int 10 00 00 00H(PCM)
	0x01, 0x00,										//14H 	2 	int 		01 00H int fmttag=0x01
	0x01, 0x00,            							//16H 	2 	int 		int channel=1��2
	0x44,0xac,0x00,0x00,  	// samplespersec 16000(0x80,0x3E,0x00,0x00) 8000(0x40, 0x1F, 0x00, 0x00)
	0x10,0xB1,0x02,0x00,							//1CH 	4 	long 		int ÿ�벥���ֽ��� long int bytepersec
	0x04,0x00,										//20H 	2 	int 		����һ��ռ�ֽ��� int blockalign=������*������/8
	0x10,0x00,										//22H 	2 	int 		������ int bitpersamples=8��16
	0x64,0x61,0x74,0x61,							//24H 	4 	char 		"data"
	0x00,0x00,0x00,0x00								//28H 	4 	long 		int ���������ֽ��� long int size2=�ĳ�-44
};
void wavset()
{
	unsigned short	channels;
	unsigned int	longSampleRate;
	unsigned int	byteRate;
	unsigned short	bitsPerSample;
	bitsPerSample = 16;			//����λ��
	channels = 1;
	longSampleRate = 8000;		//����Ƶ��
	byteRate = longSampleRate * (bitsPerSample / 8) * channels;

	WavHeadData[0] = 'R'; // RIFF
	WavHeadData[1] = 'I';
	WavHeadData[2] = 'F';
	WavHeadData[3] = 'F';
	//�ļ�����  4�ֽ��ļ����ȣ�������Ȳ�����"RIFF"��־(4�ֽ�)���ļ����ȱ�����ռ�ֽ�(4�ֽ�),���ó��ȵ��������ļ����� - 8
	WavHeadData[4] = 0;
	WavHeadData[5] = 0;
	WavHeadData[6] = 0;
	WavHeadData[7] = 0;
	//fcc type��4�ֽ� "WAVE" ���Ϳ��ʶ, ��д
	WavHeadData[8] = 'W';
	WavHeadData[9] = 'A';
	WavHeadData[10] = 'V';
	WavHeadData[11] = 'E';
	//FMT Chunk   4�ֽ� ��ʾ"fmt" chunk�Ŀ�ʼ,�˿��а����ļ��ڲ���ʽ��Ϣ��Сд, ���һ���ַ��ǿո�
	WavHeadData[12] = 'f'; // 'fmt '
	WavHeadData[13] = 'm';
	WavHeadData[14] = 't';
	WavHeadData[15] = ' ';//�����ֽ�
	//���ݴ�С  4�ֽڣ��ļ��ڲ���ʽ��Ϣ���ݵĴ�С�������ֽڣ�һ��Ϊ00000010H��
	WavHeadData[16] = 16;
	WavHeadData[17] = 0;
	WavHeadData[18] = 0;
	WavHeadData[19] = 0;
	//���뷽ʽ 10HΪPCM�����ʽ   FormatTag��2�ֽڣ���Ƶ���ݵı��뷽ʽ��1����ʾ��PCM ����
	WavHeadData[20] = 1; // format = 1
	WavHeadData[21] = 0;
	//ͨ����  Channels��2�ֽڣ���������������Ϊ1��˫����Ϊ2
	WavHeadData[22] = (byte)channels;
	WavHeadData[23] = 0;
	//�����ʣ�ÿ��ͨ���Ĳ����ٶ�
	WavHeadData[24] = (byte)(longSampleRate & 0xff);
	WavHeadData[25] = (byte)((longSampleRate >> 8) & 0xff);
	WavHeadData[26] = (byte)((longSampleRate >> 16) & 0xff);
	WavHeadData[27] = (byte)((longSampleRate >> 24) & 0xff);
	//��Ƶ���ݴ�������,������*ͨ����*�������/8
	//4�ֽڣ���Ƶ���ݴ�������, ��λ���ֽڡ���ֵΪ�����ʡ�ÿ�β�����С������������ô�ֵ���Թ��ƻ������Ĵ�С
	//byteRate = sampleRate * (bitsPerSample / 8) * channels
	WavHeadData[28] = (byte)(byteRate & 0xff);
	WavHeadData[29] = (byte)((byteRate >> 8) & 0xff);
	WavHeadData[30] = (byte)((byteRate >> 16) & 0xff);
	WavHeadData[31] = (byte)((byteRate >> 24) & 0xff);
	// ȷ��ϵͳһ��Ҫ������ٸ������ֽڵ����ݣ�ȷ����������ͨ����*����λ��
	WavHeadData[32] = (byte)(bitsPerSample * channels / 8);
	WavHeadData[33] = 0;
	//ÿ������������λ��
	//2�ֽڣ�ÿ�������Ĳ�������; Ʃ�� 16bit �������ֵ����16������ж����������ÿ�������Ĳ������ȴ�С��һ���ģ�
	WavHeadData[34] = byte(bitsPerSample);
	WavHeadData[35] = 0;
	//Data chunk
	//ckid��4�ֽڣ����ݱ�־����data������ʾ "data" chunk�Ŀ�ʼ���˿��а�����Ƶ���ݣ�Сд��
	WavHeadData[36] = 'd';
	WavHeadData[37] = 'a';
	WavHeadData[38] = 't';
	WavHeadData[39] = 'a';
	//��Ƶ���ݵĳ��ȣ�4�ֽڣ�audioDataLen = totalDataLen - 36 = fileLenIncludeWavHeadData - 44
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

		if (strstr(buff, "+NRECD: "))		//���ｫNRECD: 960,1314���������ȫ���޳�������¼��������
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