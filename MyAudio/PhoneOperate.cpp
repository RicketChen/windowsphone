#include "PhoneOperate.h"

extern CSerialPort mySerialPort;						//�����豸��Ϣ��ʼ��
extern char receive[65535];

extern PhoneInfo_t DefaultPhoneInfo;

int PhoneGetCmd(char* cmd, char* callargs, return_data_t* result)
{
	soft_PhoneOperate(&mySerialPort, cmd, callargs, result);

	return 0;
}