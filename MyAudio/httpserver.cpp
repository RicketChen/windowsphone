#include "mongoose.h"
#include "httpserver.h"
#include "stdafx.h"

using std::cout;
using std::endl;

extern PhoneInfo_t DefaultPhoneInfo;

//定义web服务端口号
static const char *s_http_port = DefaultPhoneInfo.ports;
struct mg_mgr		mgr;					// Mongoose event manager
struct mg_connection *mg_connection;					// Mongoose Connection
/** 线程ID */
UINT HttpServerThreadId;
volatile HANDLE    HD_HttpServerThread;

//post和get请求分类

static const struct	mg_str	s_get_method = MG_MK_STR("GET");

static const struct mg_str	s_post_method = MG_MK_STR("POST");

//几个辅助函数

//判断前缀

static int has_prefix(const	struct	mg_str *uri, const	struct	mg_str *prefix)
{
	return uri->len > prefix->len && memcmp(uri->p, prefix->p, prefix->len) == 0;
}

//判断相同

static int is_equal(const struct mg_str *s1, const struct mg_str *s2)
{
	return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

//get请求下的简单求和

static void handle_sum_call_get(struct	mg_connection *nc, struct http_message *hm, return_data_t* return_data) {

//	std::string	callback, callargs;

	/* Get form variables */

	/*mg_get_http_var(&hm->query_string, "callback", &callback[0], sizeof(callback));

	mg_get_http_var(&hm->query_string, "callargs", &callargs[0], sizeof(callargs));*/


	/* Send headers */

	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	mg_printf_http_chunk(nc, "%s({ \"code\": %d ,\"data\":{%s},\"msg\":\"%s\"})",return_data->callback, return_data->code, return_data->data, return_data->msg);

	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */

}

//post请求下的简单求和

static void handle_sum_call_post(struct	mg_connection *nc, struct http_message *hm)
{
	char n1[100], n2[100];

	double result;

	/* Get form variables */

	mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));

	mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));

	/* Send headers */

	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Compute the result and send it back as a JSON object */

	result = strtod(n1, NULL) + strtod(n2, NULL);

	mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);

	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */

}

//网络请求处理回调函数

//struct mg_connection *c 和客户端建立的请求链接，给客户端返回数据时需要

//int ev 应该时请求类型

//void *ev_data 客户端发送来的整个请求数据

void ev_handler(struct mg_connection *nc, int ev, void *ev_data)
{
	return_data_t* return_data;
	return_data = new return_data_t;

	static const struct mg_str api_prefix = MG_MK_STR("/+");
	static const struct mg_str api_prefix2 = MG_MK_STR("/a");

	struct http_message *hm = (struct http_message *) ev_data;

	struct mg_str key;

	switch (ev)
	{
		case MG_EV_HTTP_REQUEST:
			//校验请求地址是否符合规定的约定的前缀
			if (has_prefix(&hm->uri, &api_prefix) || has_prefix(&hm->uri, &api_prefix2))
			{
				key.p = hm->uri.p + api_prefix.len;

				key.len = hm->uri.len - api_prefix.len;

				//分方法处理
				char cmd[20];
				char callback[150];
				char callargs[150];
				if (is_equal(&hm->method, &s_get_method))
				{
					memset(cmd, 0, sizeof(cmd));
					soft_memcpy(hm->uri.p, "/", "?", cmd);
					mg_get_http_var(&hm->query_string, "callback", callback, sizeof(callback));

					mg_get_http_var(&hm->query_string, "callargs", callargs, sizeof(callargs));
					
					strcpy(return_data->callback, callback);
					PhoneGetCmd(cmd, (char*)callargs, return_data);
					mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

					mg_printf_http_chunk(nc, "%s({ \"code\": %d ,\"data\":%s,\"msg\":\"%s\"})", return_data->callback, return_data->code, return_data->data, return_data->msg);

					mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
				//	memset(&hm->query_string.p, 0, sizeof(hm->query_string.p));
				//	ev = 0;
				//	handle_sum_call_get(nc, hm, return_data);

				}
				else 
				{
					//其他请求类型的不做处理
					mg_printf(nc, "%s",	"HTTP/1.0 501 Not Implemented\r\n""Content-Length: 0\r\n\r\n");
				}
			}
			else
			{
				//请求非法时，不处理
				mg_str re = mg_mk_str("该请求无法处理");
				mg_send_head(nc, 200, re.len, "Content-Type: text/plain");
				mg_printf(nc, "%.*s", (int)re.len, re.p);
			}
			break;
	default:break;
	}
	delete return_data;
}
UINT WINAPI HttpServerThread(void* pParam)
{
	mg_mgr_init(&mgr, NULL);
	mg_connection = mg_bind(&mgr, s_http_port, ev_handler);
	LOG(INFO) << "START HTTP SERVER" << ", connect port is " << s_http_port;
	mg_set_protocol_http_websocket(mg_connection);
	while (1)
	{
		mg_mgr_poll(&mgr, 1000);
		Sleep(100);
	}
	return 0;
}
bool httpServerStart()
{
/*	if (HD_HttpServerThread != INVALID_HANDLE_VALUE)
	{
		cout << "hello" << endl;
		// 线程已经开启 
		return false;
	}*/
	HD_HttpServerThread = (HANDLE)_beginthreadex(NULL, 0, HttpServerThread, 0, 0, &HttpServerThreadId);
	return true;
}