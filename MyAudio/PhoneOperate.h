#pragma once
#ifndef _PHONE_OPERATE_H_
#define _PHONE_OPERATE_H_

#include <string>
#include "stdafx.h"
#include "WindowsPhone.h"
#include "soft_function.h"

using std::cout;
using std::endl;

int PhoneGetCmd(char* cmd, char* callargs, return_data_t* result);
#endif // !_PHONE_OPERATE_H_

