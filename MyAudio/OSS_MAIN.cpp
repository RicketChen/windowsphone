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
	/* ��char*���͵��ַ�����ʼ��aos_string_t���͡�*/
	aos_str_set(&options->config->endpoint, endpoint);
	aos_str_set(&options->config->access_key_id, access_key_id);
	aos_str_set(&options->config->access_key_secret, access_key_secret);
	/* �Ƿ�ʹ����CNAME��0��ʾ��ʹ�á�*/
	options->config->is_cname = 0;
	/* ����������ز��������糬ʱʱ��ȡ�*/
	options->ctl = aos_http_controller_create(options->pool, 0);
}
int upload(char* filename)
{
	/* �ڳ�����ڵ���aos_http_io_initialize��������ʼ�����硢�ڴ��ȫ����Դ��*/
	if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
		exit(1);
	}
	/* �����ڴ������ڴ�أ�pool�����ȼ���apr_pool_t����ʵ�ִ�����apr���С�*/
	aos_pool_t *pool;
	/* ���´���һ���ڴ�أ��ڶ���������NULL����ʾû�м̳������ڴ�ء�*/
	aos_pool_create(&pool, NULL);
	/* ��������ʼ��options���ò�������endpoint��access_key_id��acces_key_secret��is_cname��curl��ȫ��������Ϣ��*/
	oss_request_options_t *oss_client_options;
	/* ���ڴ���з����ڴ��options��*/
	oss_client_options = oss_request_options_create(pool);
	/* ��ʼ��Client��ѡ��oss_client_options��*/
	init_options(oss_client_options);
	/* ��ʼ��������*/
	aos_string_t bucket;
	aos_string_t object;
	aos_string_t file;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	aos_str_set(&bucket, bucket_name);
	aos_str_set(&object, object_name);
	aos_str_set(&file, filename);
	/* �ϴ��ļ���*/
	resp_status = oss_put_object_from_file(oss_client_options, &bucket, &object, &file, headers, &resp_headers);
	/* �ж��ϴ��Ƿ�ɹ���*/
	if (aos_status_is_ok(resp_status)) { 
	//	printf("put object from file succeeded\n");
		LOG(INFO) << "put object from file succeeded ,File nam,e is " << DefaultPhoneInfo.wavFileName << endl;
	}
	else {
		printf("put object from file failed\n");
	}
	/* �ͷ��ڴ�أ��൱���ͷ�����������и���Դ������ڴ档*/
	aos_pool_destroy(pool);
	/* �ͷ�֮ǰ�����ȫ����Դ��*/
	aos_http_io_deinitialize();
	return 0;
}