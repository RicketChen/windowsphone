#pragma once
#ifndef _OSS_MAIN_H_
#define _OSS_MAIN_H_

#include "stdafx.h"
#include "oss_api.h"
#include "aos_http_io.h"
#include "soft_function.h"

void init_options(oss_request_options_t *options);
int upload(char* filename);
#endif // !_OSS_MAIN_H_
