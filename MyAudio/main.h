#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_

#include "stdafx.h"
#include "WindowsPhone.h"
//#include "myAudio.h"
#include "enumser.h"
#include "enumport.h"
#include "OSS_MAIN.h"

#define buffNumber 2
#define LINE_SIZE 2048
CSerialPort mySerialPort;						//串口设备信息初始化
//baseMyAudio *myaudio = new myAudio(buffNumber);	//音频设备信息初始化

const char *con_ports = "ports=";
const char *con_threads = "threads=";
const char *con_endpoint = "endpoint=";
const char *con_accessKeyId = "accessKeyId=";
const char *con_accessKeySecret = "accessKeySecret=";
const char *con_agentId = "agentId=";
const char *con_company = "company=";
const char *con_mobile = "mobile=";
const char *con_comName = "comName=";
const char *con_cmdWait = "cmdWait=";
const char *con_urlStatusPush = "urlStatusPush=";
const char *con_pushWait = "pushWait=";
const char *con_logLevel = "logLevel=";
const char *con_bucket = "bucket=";

//char filename[30] = { 0 };
//char mp3FileName[30] = { 0 };
//char wavFileName[30] = { 0 };

//char configtxt[1000];		//get the config.txt

PhoneInfo_t DefaultPhoneInfo;

//static const char *s_http_port = "8055";

#endif // !_MAIN_H_
