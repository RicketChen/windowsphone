#include <string>

#include "httpserver.h"
#include "main.h"
#include "stdafx.h"
#include "txttowav.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "OSS_MAIN.h"
#include "soft_curl.h"
//#include "httpclient.h"
using namespace rapidjson;
using std::string;

#pragma comment(lib,"glog.lib")
//#pragma comment(lib,"libcurl-d_imp.lib")
#define CONTENTTYPE "Content-Type: application/x-www-form-urlencoded"
#define CONTENTTYPE_JSON "Content-Type:application/json"
char receive[65535];
FILE* mp3fp = 0;
unsigned char temp[100];
bool ReopenSerial = false;
static volatile int keepRunning = 1;
//char* configtxt = DefaultPhoneInfo.config;

//struct mg_mgr mgr_post;
void sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		system("pause");
		fclose(mp3fp);
		//		mySerialPort.~CSerialPort();
		//		mg_mgr_free(&mgr);	//释放mgr
		keepRunning = 0;
		//		cout << "i am closed" << endl;
	}
}
static int s_exit_flag = 0;
bool failpost = true;
bool finshpost = false;
static void ev_handler_post(struct mg_connection *connection, int event_type, void *event_data)
{
	http_message *hm = (struct http_message *)event_data;
	int connect_status;

	switch (event_type)
	{
	case MG_EV_CONNECT:
		connect_status = *(int *)event_data;
		if (connect_status != 0)
		{
			printf("Error connecting to server, error code: %d\n", connect_status);
			//	s_exit_flag = 1;
			failpost = true;
			finshpost = true;
		}
		break;
	case MG_EV_HTTP_REPLY:
	{
		cout << hm->body.p;
		if (strstr(hm->body.p, "success"))
		{
			failpost = false;
			finshpost = true;
			LOG(INFO) << "推送成功";
		}
		//printf("Got reply:\n%.*s\n", (int)hm->body.len, hm->body.p);
		std::string rsp = std::string(hm->body.p, hm->body.len);
		connection->flags |= MG_F_SEND_AND_CLOSE;
		//	s_exit_flag = 1; // 每次收到请求后关闭本次连接，重置标记
			// 回调处理
		//	s_req_callback(rsp);
	}
	break;
	case MG_EV_CLOSE:
		if (s_exit_flag == 0)
		{
			failpost = false;
			finshpost = true;
			//	printf("Server closed connection\n");
			s_exit_flag = 1;
		};
		break;
	default:
		break;
	}
}
void connectHhttp(const char* url, const char* contentType, const char* params) {

	struct mg_mgr *mgr_post = new mg_mgr;
	mg_mgr_init(mgr_post, NULL);
	auto connection = mg_connect_http(mgr_post, ev_handler_post, url, contentType, params);//POST模拟表单提交
	mg_set_protocol_http_websocket(connection);
	while (s_exit_flag == 0) {
		mg_mgr_poll(mgr_post, 100);
	}
	s_exit_flag = 0;//重置标志位 
	mg_mgr_free(mgr_post);
	delete mgr_post;
}
int main(int argc, char* argv[])
{
	const string pushurl = "http://apis.7moor.com/sim/cdr_ytx/insertCdr/";

	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::GLOG_INFO, "myInfo.txt");
	google::SetStderrLogging(google::GLOG_INFO);
	google::SetLogFilenameExtension("log_");
	FLAGS_colorlogtostderr = true;  // Set log color
	FLAGS_logbufsecs = 0;  // Set log output speed(s)
	FLAGS_max_log_size = 1024;  // Set max log file size
	FLAGS_stop_logging_if_full_disk = true;  // If disk is full

	LOG(INFO) << "START WORK!";

	//	google::ShutdownGoogleLogging();

	UINT portnums = 0;
	signal(SIGINT, sig_handler);

	/*	readfile("config.txt", DefaultPhoneInfo.config);		//读取配置文件内容到configtxt数组

		soft_getchar(DefaultPhoneInfo.endpoint, configtxt, (char*)con_endpoint);					//读取OSS配置，endpoint
		soft_getchar(DefaultPhoneInfo.access_key_id, configtxt, (char*)con_accessKeyId);			//读取OSS配置，accessKeyId
		soft_getchar(DefaultPhoneInfo.access_key_secret, configtxt, (char*)con_accessKeySecret);	//读取OSS配置，accessKeySecret
		soft_getchar(DefaultPhoneInfo.bucket_name, configtxt, (char*)con_bucket);					//读取OSS配置，bucket
		soft_getchar(DefaultPhoneInfo.agentId, configtxt, (char*)con_agentId);					//读取坐席ID，agnetId
		soft_getchar(DefaultPhoneInfo.company, configtxt, (char*)con_company);					//读取企业ID，company
		soft_getchar(DefaultPhoneInfo.mobile, configtxt, (char*)con_mobile);						//读取本机号码，mobile
		soft_getchar(DefaultPhoneInfo.ports, configtxt, "ports=");*/

		//	txt_to_wav();
		//	while (1);

	httpServerStart();				//启动http服务

	portnums = getport();			//获取话机串口的COM号，
//	cin >> portnums;
	while (!mySerialPort.InitPort(portnums, 460800, 'N', 8, 1, EV_RXCHAR))
	{
		portnums = getport();			//获取话机串口的COM号，
		LOG(INFO) << "initializing com " << portnums << endl;

		Sleep(1000);
	}

	LOG(INFO) << "open com success!" << std::endl;

	if (!mySerialPort.OpenListenThread())
	{
		LOG(INFO) << "OpenListenThread fail !" << std::endl;
	}
	else
	{
		LOG(INFO) << "OpenListenThread success !" << std::endl;
	}
	mySerialPort.WriteData((unsigned char *)"at+cgsn\r\n", strlen("at+cgsn\r\n"));

	memset(receive, 0, sizeof(receive));

	int i = 0;
	do
	{
		if (strstr((char*)receive, "at+cgsn"))
		{
			break;
		}
		Sleep(100);
	} while (1);
	strncpy(DefaultPhoneInfo.imei, strstr(receive, "at+cgsn\r\n\r\n") + strlen("at+cgsn\r\n\r\n"), strlen("123456789123456"));
	memset(receive, 0, sizeof(receive));
	bool sendphonecall = false;
	DefaultPhoneInfo.status = DefaultPhoneInfo.idleing;
	while (1)
	{
		if (strstr((char*)receive, "+OUTGOINGCALL:\r\n+OUTGOINGCALL:"))
		{
			DefaultPhoneInfo.callin[0] = '0';
			sprintf(DefaultPhoneInfo.callid, "%s-%s-%s-%s", soft_GetNowTime("%Y%m%d%H%M%S").c_str(), DefaultPhoneInfo.agentId, DefaultPhoneInfo.company, DefaultPhoneInfo.mobile);
			sendphonecall = true;
			DefaultPhoneInfo.status = DefaultPhoneInfo.calling;
			strcpy(DefaultPhoneInfo.calltime, soft_GetNowTime("%Y%m%d%H%M%S").c_str());

			mp3fp = fopen("InputData.txt", "wb+");
			if (mp3fp > 0)
			{
				LOG(INFO) << "CREATED INPUTDATA FILE";
			}
			else
			{
				LOG(ERROR) << "CREATE INPUTDATA FAIL!";
				return 0;
			}
			memset(receive, 0, sizeof(receive));
			LOG(INFO) << " CALLING " << DefaultPhoneInfo.called;
		}
		else if (strstr((char*)receive, "+NTALK: 1"))
		{
			DefaultPhoneInfo.status = DefaultPhoneInfo.calling;
			memset(receive, 0, sizeof(receive));
			LOG(INFO) << " CONNECT BUILT ";
		}
		else if (strstr((char*)receive, "+NTALK: 2"))
		{
			sendphonecall = true;
			DefaultPhoneInfo.starttime = soft_getnowtime_int();
			DefaultPhoneInfo.status = DefaultPhoneInfo.talking;
			strcpy(DefaultPhoneInfo.talktime, soft_GetNowTime("%Y%m%d%H%M%S").c_str());
			sprintf(DefaultPhoneInfo.wavFileName, "%s-%s-%s-%s.wav", DefaultPhoneInfo.talktime, DefaultPhoneInfo.agentId, DefaultPhoneInfo.company, DefaultPhoneInfo.imei);
			memset(receive, 0, sizeof(receive));
			LOG(INFO) << " recording! recoed file is " << DefaultPhoneInfo.wavFileName;
		}
		else if (strstr((char*)receive, "+NTALK: 0"))
		{
			sendphonecall = true;
			strcpy(DefaultPhoneInfo.cendtime, soft_GetNowTime("%Y%m%d%H%M%S").c_str());
			DefaultPhoneInfo.stoptime = soft_getnowtime_int();
			DefaultPhoneInfo.talk_seconds = (int)(DefaultPhoneInfo.stoptime - DefaultPhoneInfo.starttime);
			if (!strcmp(DefaultPhoneInfo.talktime, ""))
			{
				DefaultPhoneInfo.talk_seconds = 0;
			}
			DefaultPhoneInfo.status = DefaultPhoneInfo.hankup;
			LOG(INFO) << " CONNECT CLOSED ";
			memset(receive, 0, sizeof(receive));
		}
		else if (strstr((char*)receive, "+EAIC: 1,"))
		{
			int len = 0;
			DefaultPhoneInfo.callin[0] = '1';
			sprintf(DefaultPhoneInfo.callid, "%s-%s-%s-%s", soft_GetNowTime("%Y%m%d%H%M%S").c_str(), DefaultPhoneInfo.agentId, DefaultPhoneInfo.company, DefaultPhoneInfo.mobile);
			mp3fp = fopen("InputData.txt", "wb+");
			if (mp3fp > 0)
			{
				LOG(INFO) << "CREATED INPUTDATA FILE";
			}
			char *temp = strstr((char*)receive, "+EAIC: 1, ") + strlen("+EAIC: 1, \"");

			while ((*(temp + len)) != '\"')
			{
				DefaultPhoneInfo.called[len] = temp[len++];
			}
			strcpy(DefaultPhoneInfo.calltime, soft_GetNowTime("%Y%m%d%H%M%S").c_str());
			sprintf(DefaultPhoneInfo.wavFileName, "%s-%s-%s-%s-%s.wav", DefaultPhoneInfo.talktime, DefaultPhoneInfo.agentId, DefaultPhoneInfo.company, DefaultPhoneInfo.mobile, DefaultPhoneInfo.called);
			LOG(INFO) << " RINGING!!   GOT A CALL FROM " << DefaultPhoneInfo.called;

			memset(receive, 0, sizeof(receive));
		}
		if (sendphonecall)
		{
			sendphonecall = false;
			char phone_status[500];
			StringBuffer phone_status_json;
			Writer<rapidjson::StringBuffer> writer(phone_status_json);
			phone_status_json.Clear();
			writer.StartObject();
			writer.Key("type");
			writer.String("status");
			writer.Key("data");
			writer.StartObject();
			writer.Key("imei");
			writer.String(DefaultPhoneInfo.imei);
			writer.Key("agnetId");
			writer.String(DefaultPhoneInfo.agentId);
			writer.Key("company");
			writer.String(DefaultPhoneInfo.company);
			writer.Key("mobile");
			writer.String(DefaultPhoneInfo.mobile);

			writer.Key("status");
			writer.String(DefaultPhoneInfo.status);

			writer.Key("timestamp");
			writer.String(soft_GetNowTime("%Y%m%d%H%M%S").c_str());
			writer.Key("callin");
			writer.String(DefaultPhoneInfo.callin);			//来电状态，后期需要改成可判断来电或去电，目前写死为0
			//writer.String("1");			//来电状态，后期需要改成可判断来电或去电，目前写死为0
			writer.Key("useapi");
			writer.String("0");
			writer.Key("callid");
			writer.String(DefaultPhoneInfo.callid);
			writer.Key("number");
			writer.String(DefaultPhoneInfo.called);
			writer.Key("call_time");
			writer.String(DefaultPhoneInfo.calltime);
			if (!strcmp(DefaultPhoneInfo.status, "calling"))
			{

			}
			else
			{
				writer.Key("talk_time");
				writer.String(DefaultPhoneInfo.talktime);
				writer.Key("record_file");
				writer.String(DefaultPhoneInfo.wavFileName);
			}
			writer.Key("id");
			writer.String("");
			writer.Key("customer_sample");
			writer.String("");
			writer.EndObject();

			writer.EndObject();
			sprintf(phone_status, "%s%s", "notification=", phone_status_json.GetString());
			/*post:
						connectHhttp("http://apis.7moor.com/sim/cdr_ytx/insertCdr/", CONTENTTYPE, phone_status_json.GetString());
						while (finshpost)
						{
							finshpost = false;
							if (failpost)
								goto post;
							else
							{
								failpost = true;
								break;
							}
						}*/
			soft_curlpost(pushurl, CONTENTTYPE, phone_status_json.GetString());
			LOG(INFO) << "推送状态: " << DefaultPhoneInfo.status;
			LOG(INFO) << phone_status_json.GetString();

		}
		if (!strcmp(DefaultPhoneInfo.status, "hankup"))
		{
			DefaultPhoneInfo.status = DefaultPhoneInfo.idleing;
			char phone_cdr[500];
			StringBuffer phone_cdr_json;
			Writer<rapidjson::StringBuffer> writer(phone_cdr_json);
			phone_cdr_json.Clear();
			writer.StartObject();
			writer.Key("type");
			writer.String("cdr");
			writer.Key("data");
			writer.StartObject();

			writer.Key("imei");
			writer.String(DefaultPhoneInfo.imei);
			writer.Key("agnetId");
			writer.String(DefaultPhoneInfo.agentId);
			writer.Key("company");
			writer.String(DefaultPhoneInfo.company);
			writer.Key("mobile");
			writer.String(DefaultPhoneInfo.mobile);

			writer.Key("callin");
			writer.String(DefaultPhoneInfo.callin);
			//	writer.String("1");
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
			writer.Key("cend_time");
			writer.String(DefaultPhoneInfo.cendtime);
			writer.Key("talk_seconds");
			char talktime[10];
			sprintf(talktime, "%d", DefaultPhoneInfo.talk_seconds);
			writer.String(talktime);

			writer.Key("id");
			writer.String("");
			writer.Key("customer_sample");
			writer.String("");

			writer.EndObject();

			writer.EndObject();
			sprintf(phone_cdr, "%s%s", "notification=", phone_cdr_json.GetString());


			//connectHhttp("http://apis.7moor.com/sim/cdr_ytx/insertCdr/", CONTENTTYPE, phone_cdr_json.GetString());

			soft_curlpost(pushurl, CONTENTTYPE, phone_cdr_json.GetString());
			DefaultPhoneInfo.callin[0] = '2';
			LOG(INFO) << "推送话单";
			LOG(INFO) << phone_cdr_json.GetString();
			memset(DefaultPhoneInfo.called, 0, sizeof(DefaultPhoneInfo.called));
			memset(DefaultPhoneInfo.callid, 0, sizeof(DefaultPhoneInfo.callid));
			memset(DefaultPhoneInfo.calltime, 0, sizeof(DefaultPhoneInfo.calltime));
			memset(DefaultPhoneInfo.cendtime, 0, sizeof(DefaultPhoneInfo.cendtime));
			DefaultPhoneInfo.starttime = 0;
			DefaultPhoneInfo.stoptime = 0;
			memset(DefaultPhoneInfo.talktime, 0, sizeof(DefaultPhoneInfo.talktime));
			memset(DefaultPhoneInfo.wavFileName, 0, sizeof(DefaultPhoneInfo.wavFileName));
		}
		//		return 0;
	}
	return 0;
}