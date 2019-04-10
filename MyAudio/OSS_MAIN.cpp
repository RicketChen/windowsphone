#include "OSS_MAIN.h"
//const char *endpoint = "oss-cn-hangzhou.aliyuncs.com";
//const char *access_key_id = "LTAIIAmJcfIHwqQ1";
//const char *access_key_secret = "jkQ4zKzkU3bfVIlPSoZtykyKgJbii9";
//const char *bucket_name = "m7-sim-cdr-record";
//const char *object_name = "<yourObjectName>";

extern char* configtxt;
extern PhoneInfo_t DefaultPhoneInfo;
char* endpoint = DefaultPhoneInfo.endpoint;
char* access_key_id = DefaultPhoneInfo.access_key_id;
char* access_key_secret = DefaultPhoneInfo.access_key_secret;
char* bucket_name = DefaultPhoneInfo.bucket_name;
char* object_name= DefaultPhoneInfo.wavFileName;
extern const char *con_endpoint;
extern const char *con_accessKeyId;
extern const char *con_accessKeySecret;
extern const char *con_bucket;
void init_options(oss_request_options_t *options)
{
	options->config = oss_config_create(options->pool);
	/* 用char*类型的字符串初始化aos_string_t类型。*/
	aos_str_set(&options->config->endpoint, endpoint);
	aos_str_set(&options->config->access_key_id, access_key_id);
	aos_str_set(&options->config->access_key_secret, access_key_secret);
	/* 是否使用了CNAME。0表示不使用。*/
	options->config->is_cname = 0;
	/* 设置网络相关参数，比如超时时间等。*/
	options->ctl = aos_http_controller_create(options->pool, 0);
}
int upload(char* filename)
{
	/* 在程序入口调用aos_http_io_initialize方法来初始化网络、内存等全局资源。*/
	if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
		exit(1);
	}
	/* 用于内存管理的内存池（pool），等价于apr_pool_t。其实现代码在apr库中。*/
	aos_pool_t *pool;
	/* 重新创建一个内存池，第二个参数是NULL，表示没有继承其它内存池。*/
	aos_pool_create(&pool, NULL);
	/* 创建并初始化options，该参数包括endpoint、access_key_id、acces_key_secret、is_cname、curl等全局配置信息。*/
	oss_request_options_t *oss_client_options;
	/* 在内存池中分配内存给options。*/
	oss_client_options = oss_request_options_create(pool);
	/* 初始化Client的选项oss_client_options。*/
	init_options(oss_client_options);
	/* 初始化参数。*/
	aos_string_t bucket;
	aos_string_t object;
	aos_string_t file;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	aos_str_set(&bucket, bucket_name);
	aos_str_set(&object, object_name);
	aos_str_set(&file, filename);
	/* 上传文件。*/
	resp_status = oss_put_object_from_file(oss_client_options, &bucket, &object, &file, headers, &resp_headers);
	/* 判断上传是否成功。*/
	if (aos_status_is_ok(resp_status)) { 
	//	printf("put object from file succeeded\n");
		LOG(INFO) << "put object from file succeeded ,File nam,e is " << DefaultPhoneInfo.wavFileName << endl;
	}
	else {
		printf("put object from file failed\n");
	}
	/* 释放内存池，相当于释放了请求过程中各资源分配的内存。*/
	aos_pool_destroy(pool);
	/* 释放之前分配的全局资源。*/
	aos_http_io_deinitialize();
	return 0;
}