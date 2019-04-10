#include "oss_c_sdk\include\curl\curl.h"
#include "soft_curl.h"

//#pragma comment(lib,"libcurl-d_imp.lib")

//#pragma comment(lib,"curl.lib")
using namespace std;

struct MemoryStruct
{
	char* memory;
	size_t size;
	MemoryStruct()
	{
		memory = (char*)malloc(1);
		size = 0;
	}
	~MemoryStruct()
	{
		free(memory);
		memory = NULL;
		size = 0;
	}
};

size_t WriteMemoryCallback(void* ptr, size_t size, size_t nmemb, void* data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)data;

	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory)
	{
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

int soft_curlpost(string url,char* content,const char* param)
{
	CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != res)
	{
		cout << "curl init failed" << endl;
		return 1;
	}

	CURL* pCurl;
	pCurl = curl_easy_init();

	if (NULL == pCurl)
	{
		cout << "Init CURL failed..." << endl;
		return -1;
	}

	curl_slist* pList = NULL;
//	pList = curl_slist_append(pList, "Content-Type:application/json");
//	content = "Content-Type:application/json";
	pList = curl_slist_append(pList, content);
//	pList = curl_slist_append(pList, "Accept:application/json, text/javascript, */*; q=0.01");
//	pList = curl_slist_append(pList, "Accept-Language:zh-CN,zh;q=0.8");
	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pList);
		
	curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str()); //提交表单的URL地址

	curl_easy_setopt(pCurl, CURLOPT_HEADER, 0L);  //启用时会将头文件的信息作为数据流输
	curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);//允许重定向
	curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1L);

	//将返回结果通过回调函数写到自定义的对象中
	MemoryStruct oDataChunk;
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &oDataChunk);
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L); //启用时会汇报所有的信息
	//post表单参数
	string strJsonData;
	strJsonData = "queryWord=CONTENT:码农&";
	strJsonData += "startTime=1507605327&";
	strJsonData += "endTime=1508210127&";
	strJsonData += "maxCount=500&";
	strJsonData += "contextLength=200";
	//libcur的相关POST配置项
	curl_easy_setopt(pCurl, CURLOPT_POST, 1L);
	curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, param);
	curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, strlen(param));


	res = curl_easy_perform(pCurl);

	long res_code = 0;
	res = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &res_code);

	if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
	{
		char* fTmpMem = (char*)oDataChunk.memory;
	/*	if (!fTmpMem) {

		}*/
		cout << "receive data is "<< fTmpMem;
		oDataChunk.~MemoryStruct();
		return true;
	}

	curl_slist_free_all(pList);
	curl_easy_cleanup(pCurl);
	curl_global_cleanup();

	return 0;
}