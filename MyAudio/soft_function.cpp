#include "stdafx.h"
#include "soft_function.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
using namespace rapidjson;

//extern CSerialPort mySerialPort;						//串口设备信息初始化
extern char receive[65535];
extern PhoneInfo_t DefaultPhoneInfo;
extern FILE* mp3fp;
int soft_cmd_cdv(CSerialPort* serialport,char* args, return_data_t* result)
{
	LOG(INFO) << "cdv request";
	char send[20] = { 0 };
	sprintf(send, "%s%s;\r\n", "ATD", args);
	strcpy(DefaultPhoneInfo.called, args);
	serialport->WriteData((unsigned char *)send, strlen(send));
	
	int i = 0;
	while (++i)
	{
		if (strstr((char*)receive, "+OUTGOINGCALL:"))
		{
			break;
		}
		if (i >= 5)
		{
			strcpy(result->msg, "adv over time");
			return -1;
		}
		Sleep(100);
	}
	memset(result->data, 0, sizeof(result->data));
	strcpy(result->msg, "OK");
	strcpy(result->data, "");
	return 0;
}
int soft_cmd_chv(CSerialPort* serialport, char* args, return_data_t* result)
{
	LOG(INFO) << "chv request";
	serialport->WriteData((unsigned char *)"ATH\r\n", strlen("ATH\r\n"));

/*	int i = 0;
	while (++i)
	{
		if (strstr((char*)receive, "OK"))
		{
			break;
		}
		if (i >= 15)
		{
			strcpy(result->msg, "over time");
			return -1;
		}
		Sleep(500);
	}*/
	memset(result->data, 0, sizeof(result->data));
	strcpy(result->msg, "OK");
	strcpy(result->data, "");
	memset(receive, 0, sizeof(receive));
	return 0;
}
int soft_cmd_vmeid(CSerialPort* serialport, char* args, return_data_t* result)
{
	LOG(INFO) << "vmeid request";
	char imei[16] = { 0 };
//	serialport->WriteData((unsigned char *)"at+vmeid?\r\n", strlen("at+vmeid\r\n"));
	serialport->WriteData((unsigned char *)"at+cgsn\r\n", strlen("at+cgsn\r\n"));

	int i = 0;
	while (++i)
	{
		if (strstr((char*)receive, "at+cgsn"))
		{
			break;
		}
		if (i >= 5)
		{
			strcpy(result->msg, "over time");
			return -1;
		}
		Sleep(100);
	}

	memset(result->data, 0, sizeof(result->data));
	strcpy(result->msg, "OK");
	strncpy(imei, strstr(receive, "at+cgsn\r\n\r\n") + strlen("at+cgsn\r\n\r\n"), strlen("123456789123456"));
	sprintf(result->data, "\"resutl\":\"%s\"", imei);
	memset(receive, 0, sizeof(receive));
	return 0;
}
int soft_cmd_ata(CSerialPort* serialport, char* args, return_data_t* result)
{
	LOG(INFO) << "ata request";
	serialport->WriteData((unsigned char *)"ATA\r\n", strlen("ATA\r\n"));
	int i = 0;
	while (++i)
	{
		if (strstr((char*)receive, "OK") || strstr((char*)receive, "NO CARRIER"))
		{
			break;
		}
		if (i >= 5)
		{
			strcpy(result->msg, "over time");
			return -1;
		}
		Sleep(100);
	}
	memset(result->data, 0, sizeof(result->data));
	strcpy(result->msg, "OK");
	strcpy(result->data, "");
	memset(receive, 0, sizeof(receive));
	return 0;
}
int soft_cmd_setconfig(CSerialPort* serialport, char* args, return_data_t* result)
{
	LOG(INFO) << "setconfig request";
	int i = 0, j = 0;
//	while ((args[i] <= '9' && args[i] >= '0'))
	while (args[i] != 0)
	{
		if (args[i] == ',')
			break;
		DefaultPhoneInfo.agentId[i-j] = args[i++];
	}
	DefaultPhoneInfo.agentId[i - j] = '\0';
	i++;
	j = i;
//	while ((args[i] <= '9' && args[i] >= '0'))
	while (args[i] != 0)
	{
		if (args[i] == ',')
			break;
		DefaultPhoneInfo.company[i-j] = args[i++];
	}
	DefaultPhoneInfo.company[i - j] = '\0';
	i++;
	j = i;
//	while ((args[i] <= '9' && args[i] >= '0'))
	while (args[i] != 0)
	{
		if (args[i] == ',')
			break;
		DefaultPhoneInfo.mobile[i-j] = args[i++];
	}
	DefaultPhoneInfo.mobile[i - j] = '\0';

/*	char* point;
	char line[1000];
	char buff[1000];
	char head[1000];
	char tail[1000];
	point = head;
	std::ifstream configtxt("config.txt");
	while (!configtxt.eof())
	{
		configtxt.getline(line, sizeof(line));
		if (strstr(line, "agentId=") || strstr(line, "company="))
		{
			continue;
		}
		else if (strstr(line, "mobile="))
		{
			point = tail;
			continue;
		}
		else
		{
			strcat(line, "\n");
			strcat(point, line);
			memset(line, 0, sizeof(line));
		}
	}
	configtxt.close();
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "agentId=%s\ncompany=%s\nmobile=%s\n", DefaultPhoneInfo.agentId, DefaultPhoneInfo.company, DefaultPhoneInfo.mobile);
	FILE* fp2 = fopen("config.txt", "w");
	fseek(fp2, 0, SEEK_SET);
	fwrite(head, sizeof(char), strlen(head), fp2);
	fwrite(buff, sizeof(char), strlen(buff), fp2);
	fwrite(tail, sizeof(char), strlen(tail), fp2);

	fclose(fp2);*/

	memset(result->data, 0, sizeof(result->data));
	strcpy(result->msg, "OK");
	strcpy(result->data, "");
	memset(receive, 0, sizeof(receive));

	return 0;
}
int soft_cmd_trans(CSerialPort* serialport, char* args, return_data_t* result)
{
//	trans();
	return 0;
}
int soft_cmd_telstatus(CSerialPort* serialport, char* args, return_data_t* result)
{
	StringBuffer id;
	Writer<rapidjson::StringBuffer> writer(id);
	writer.StartObject();

	writer.Key("agentId");
	writer.String(DefaultPhoneInfo.agentId);
	writer.Key("company");
	writer.String(DefaultPhoneInfo.company);
	writer.Key("mobile");
	writer.String(DefaultPhoneInfo.mobile);
	writer.Key("status");
	writer.String(DefaultPhoneInfo.status);
	writer.Key("timestamp");
	writer.String(soft_GetNowTime("%Y%m%d%H%M%S").c_str());
	if (!strcmp(DefaultPhoneInfo.status, "calling"))
	{
		writer.Key("callin");
		writer.String("1");
		writer.Key("useapi");
		writer.String("0");
		writer.Key("callid");
		writer.String(DefaultPhoneInfo.callid);
		writer.Key("number");
		writer.String(DefaultPhoneInfo.called);
		writer.Key("calltime");
		writer.String(DefaultPhoneInfo.calltime);
	}
	else if (!strcmp(DefaultPhoneInfo.status, "talking"))
	{
		writer.Key("callin");
		writer.String("1");
		writer.Key("useapi");
		writer.String("0");
		writer.Key("callid");
		writer.String(DefaultPhoneInfo.callid);
		writer.Key("number");
		writer.String(DefaultPhoneInfo.called);
		writer.Key("call_time");
		writer.String(DefaultPhoneInfo.calltime);
		writer.Key("talk_time");
		writer.String(DefaultPhoneInfo.talktime);
		writer.Key("record_file");
		writer.String(DefaultPhoneInfo.wavFileName);
	}
	writer.EndObject();

	strcpy(result->data, id.GetString());
	strcpy(result->msg, "OK");

	LOG(INFO) << "telstatus request";

	return 0;
}
int soft_cmd_csq(CSerialPort* serialport, char* args, return_data_t* result)
{
	LOG(INFO) << "csq request";
	char csq[20];
	serialport->WriteData((unsigned char *)"at+csq\r\n", strlen("at + csq\r\n"));
	int i = 0;
	while (++i)
	{
		if (strstr((char*)receive, "+CSQ: "))
		{
			break;
		}
		if (i >= 5)
		{
			strcpy(result->msg, "csq over time");
			return -1;
		}
		Sleep(100);
	}
	memset(result->data, 0, sizeof(result->data));
	strcpy(result->msg, "OK");
	char *temp = strstr(receive, "+CSQ: ") + strlen("+CSQ: ");
	int len = 0;
	while (*temp != '\n')
	{
		if (*temp >= '0' && *temp <= '9' || *temp == ',')
		{
			csq[len++] = *temp;
		}
		temp++;
	}
	csq[len] = 0;
	sprintf(result->data, "{\"resutl\":\"%s\"}", csq);
	memset(receive, 0, sizeof(receive));
	return 0;
}
int soft_cmd_reopencom(CSerialPort* serialport, char* args, return_data_t* result)
{
	LOG(INFO) << "reopencom request";
	memset(result->data, 0, sizeof(result->data));
	strcpy(result->msg, "OK");
	strcpy(result->data, "{}");
	return 0;
}
cmd_array_t cmd_array[] = {
	{"+cdv"			,"",soft_cmd_cdv},
	{"+chv"			,"",soft_cmd_chv},
	{"+vmeid"		,"",soft_cmd_vmeid},
	{"ata"			,"",soft_cmd_ata},
	{"+setconfig"	,"",soft_cmd_setconfig},
	{"+trans"		,"",soft_cmd_trans},
	{"+telstatus"	,"",soft_cmd_telstatus},
	{"+csq"			,"",soft_cmd_csq},
	{"+reopencom"	,"",soft_cmd_reopencom}
};

void soft_PhoneOperate(CSerialPort* serialport, char* cmd, char* args, return_data_t* result)
{
//	result->data = "123";
//	result->msg = "OK";
	for (int i = 0; i < sizeof(cmd_array) / sizeof(cmd_array_t); i++)
	{
		if (!strcmp(cmd_array[i].cmd, cmd))
		{
			result->code = cmd_array[i].comtel_cmd_function(serialport,args, result);
		}
	}
}
void soft_memcpy(const char* src, char* head, char* tail, char* buff)
{
	memcpy(buff, strstr(src, head) + strlen(head), strstr(src, tail) - strstr(src, head) - strlen(head));
}
std::string soft_GetNowTime(char* format)
{
	std::string time;

	CString str; //获取系统时间
	CTime tm; 
	tm = CTime::GetCurrentTime();
	str = tm.Format(format);
	time = str;
	return time;
}
int soft_getchar(char* dest,char* src,char* find)
{
	int i = 0;
	char *temp = new char[strlen(src)];
	strcpy(temp, strstr(src, (char*)find) + strlen(find));
	while (temp[i] != '\n')
		dest[i] = temp[i++];

	delete [] temp;
	return 0;
}

void readfile(const char* filename, char* buff)
{
	char ch;
	int i = 0;
	FILE* configtxtfp = fopen(filename, "r");
	while ((ch = fgetc(configtxtfp)) != EOF)
		if (ch >= 0)                        //按字符方式读取文件，每读一个字符判断一下是否为汉字编码(<0)，若是则存入字符数组，不是则跳过读取下一个字符，直到文件结束。 
			buff[i++] = ch;
	fclose(configtxtfp);
}
time_t soft_getnowtime_int()
{
	time_t nowtime;
	nowtime = time(NULL); //获取当前时间
	return nowtime;
}
//字节流转换为十六进制字符串的另一种实现方式  
void soft_Hex2Str(const char *sSrc, char *sDest, int nSrcLen)
{
	short i;
	unsigned char highByte, lowByte;

	for (i = 0; i < nSrcLen; i++)
	{
		highByte = sSrc[i] >> 4;
		lowByte = sSrc[i] & 0x0f;

		highByte += 0x30;

		if (highByte > 0x39)
			sDest[i * 2] = highByte + 0x07;
		else
			sDest[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			sDest[i * 2 + 1] = lowByte + 0x07;
		else
			sDest[i * 2 + 1] = lowByte;
	}
	return;
}