#pragma once
#ifndef _SOFT_FUNCTION_H_
#define _SOFT_FUNCTION_H_

#include "WindowsPhone.h"
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <ctime>
#include <cstdio>
#include <atltime.h>
using std::cout;
using std::endl;

typedef struct _return_data_t {
	char callback[100];
	int code;
	char data[1000];
	char msg[10];
}return_data_t;

typedef int (comtel_cmd)(CSerialPort*, char*, return_data_t*);

typedef struct _cmd_array_t {
	char* cmd;
	char* args;
	comtel_cmd* comtel_cmd_function;
}cmd_array_t;


/*typedef struct configtxt_t {

	char filename[30] = { 0 };
	char mp3FileName[30] = { 0 };
	char wavFileName[30] = { 0 };

	char agentId[30] = { 0 };
	char company[30] = { 0 };
	char mobile[30] = { 0 };
};*/

void soft_PhoneOperate(CSerialPort* serialport, char* cmd, char* args, return_data_t* result);
void soft_memcpy(const char* src, char* head, char* tail, char* buff);
std::string soft_GetNowTime(char* format);
/*	
	//从src中读取find字串所在行后面的内容例如：src=abc:bcd,find=abc,则dest=:bcd
*/
int soft_getchar(char* dest, char* src, char* find);

void readfile(const char* filename, char* buff);	//读取filename文件内容，存放到buff数组中
void soft_Hex2Str(const char *sSrc, char *sDest, int nSrcLen);
time_t soft_getnowtime_int();
#endif // !_SOFT_FUNCTION_H_

