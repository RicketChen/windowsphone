#pragma once
#ifndef _HTTP_SERVER_H_
#define	_HTTP_SERVER_H_

#include <iostream>
#include <string>
#include "mongoose.h"
#include "PhoneOperate.h"

using std::cout;
using std::endl;
void ev_handler(struct mg_connection *nc, int ev, void *ev_data);


bool httpServerStart();
#endif // ! _HTTP_SERVER_H_
