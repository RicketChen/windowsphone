#pragma once

#ifndef _SOFT_CURL_H_
#define _SOFT_CURL_H_

#include <iostream>
#include <Windows.h>
#include <string>

int soft_curlpost(std::string url, char* content, const char* param);


#endif // !_SOFT_CURL_H_
