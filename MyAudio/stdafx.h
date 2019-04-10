#pragma once
#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <Windows.h>
#include <iostream>
#include <string>
#include <atlstr.h>
#include "soft_function.h"
#include "logging.h"
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "setupapi.lib")


typedef struct _PhoneInfo_t {

	char* linking = "linking";
	char* talking = "talking";
	char* idleing = "idleing";
	char* calling = "calling";
	char* hankup = "hankup";
	char* status = idleing;
	char ports[10] = "10080";
	char endpoint[50] = "oss-cn-hangzhou.aliyuncs.com";
	char access_key_id[50] = "LTAIIAmJcfIHwqQ1";
	char access_key_secret[50] = "jkQ4zKzkU3bfVIlPSoZtykyKgJbii9";
	char bucket_name[50] = "m7-sim-cdr-record";
	char object_name[50] = { "object" };

	char agentId[30] = "NOTSET";
	char company[30] = "NOTSET";
	char mobile[30] = "NOTSET";
	char called[30] = { 0 };
	char callid[120] = { 0 };

	char filename[50] = { 0 };
	char mp3FileName[50] = { 0 };
	char wavFileName[100] = { 0 };

	char calltime[15] = { 0 };
	char talktime[15] = { 0 };
	char cendtime[15] = { 0 };
	char recordpath[50] = { 0 };
	char imei[50] = { 0 };

	int talk_seconds = 0;
	time_t starttime;
	time_t stoptime;

	char callin[2] = "2";

}PhoneInfo_t;

#endif // !_STDAFX_H_
